/*
  pcidrv.c
        - PCI driver operations

  Copyright 2019  Dubrovin Egor <dubrovin.en@ya.ru>
                  Alex Popov <alexpopov@bmsru.ru>
                  Bauman Moscow State Technical University
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* Define local logging object - current part of driver */
#undef LOG_OBJECT
#define LOG_OBJECT "PCI driver"

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/interrupt.h>

#include "spu.h"
#include "log.h"
#include "info.h"
#include "pcidrv.h"

/***************************************
  Internal declarations
***************************************/

/* PCI driver privates */
static void __iomem *pci_iomem = NULL; // PCI device IO memory pointer
static u8 revision             = 0;    // PCI device revision number

/* PCI driver probe and remove functions */
static int pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent);
static void pci_driver_remove(struct pci_dev *pdev);
static irqreturn_t pci_driver_irq_handler(int irq, void *pdev);

/* Internal functions */
static int read_device_config(struct pci_dev *pdev);
static void pci_release_device(struct pci_dev *pdev);

/* IDs of supported PCI devices */
static struct pci_device_id pci_driver_ids[] =
{
  { PCI_DEVICE(VENDOR_ID, DEVICE_ID), },
  { 0, }
};

/* Export PCI devices info into user space */
MODULE_DEVICE_TABLE(pci, pci_driver_ids);

/* PCI driver definition */
static struct pci_driver pci_driver =
{
  .name     = DRIVER_NAME,
  .id_table = pci_driver_ids,
  .probe    = pci_driver_probe,
  .remove   = pci_driver_remove
};



/***************************************
  Create and destroy driver functions
***************************************/

/* Create PCI driver foreign call function */
int create_pci_driver(void)
{
  return pci_register_driver(&pci_driver);
}

/* Destroy PCI driver foreign call function */
void destroy_pci_driver(void)
{
  pci_unregister_driver(&pci_driver);
}



/***************************************
  Interface functions
***************************************/

/* Get PCI device revision */
u8 pci_get_revision(void)
{
  return revision;
}

/* Single PCI device memory write */
inline void pci_single_write(u32 data, u32 addr_shift)
{
  LOG_DEBUG("Writing value 0x%08x to address 0x%02x", data, addr_shift);
  iowrite32(data, pci_iomem + (addr_shift<<ADDR_SHIFT));
}

/* Single PCI device memory read */
inline u32 pci_single_read(u32 addr_shift)
{
  u32 data;

  /* Reading */
  data = ioread32(pci_iomem + (addr_shift<<ADDR_SHIFT));
  LOG_DEBUG("Read value 0x%08x from address 0x%02x", data, addr_shift);

  return data;
}

/* Multiple PCI device memory write */
void pci_burst_write(const struct pci_burst *pci_burst)
{
  u8 i;
  LOG_DEBUG("Writing %d words", pci_burst->count);

  for(i=0; i<pci_burst->count; i++)
  {
    pci_single_write(pci_burst->data[i], pci_burst->addr_shift[i]);
  }
}

/* Multiple PCI device memory read */
void pci_burst_read(const struct pci_burst *pci_burst)
{
  u8 i;
  LOG_DEBUG("Reading %d words", pci_burst->count);

  for(i=0; i<pci_burst->count; i++)
  {
    // Brust-getter should provide empty array to write data in
    pci_burst->data[i] = pci_single_read(pci_burst->addr_shift[i]);
  }
}



/***************************************
  Internal functions
***************************************/

/* Function called on PCI driver register */
static int pci_driver_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
  int bar, err;
  unsigned long mmio_start,mmio_len;
  u32 cntl_reg_0  = 0x0, cntl_reg_1  = 0x0; // SPU control registers to be sent

  /* Check config */
  if (read_device_config(pdev) < 0) {
    return -EIO;
  }

  /* Create BAR memory mask */
  bar = pci_select_bars(pdev, IORESOURCE_MEM);
  LOG_DEBUG("Available MEM BAR %d", bar);

  /* Enable device */
  err = pci_enable_device_mem(pdev);
  if (err)
  {
    LOG_ERROR("Failed to enable memory");
    return err;
  }
  LOG_DEBUG("Memory enabled");

  /* Request mem region for PCI device */
  err = pci_request_region(pdev, bar, DRIVER_NAME);
  if (err)
  {
    LOG_ERROR("Failed to request memory");
    pci_disable_device(pdev);
    return err;
  }
  LOG_DEBUG("Memory requested");

  /* Get IO memory region */
  mmio_start = pci_resource_start(pdev, 0);
  mmio_len   = pci_resource_len(pdev, 0);
  if(!mmio_len)
  {
    LOG_ERROR("Failed to get IO region");
    return -ENOMEM;
  }
  LOG_DEBUG("Resource 0: start at 0x%08lx with lenght %lu", mmio_start, mmio_len);

  /* Try to map IO memory into pointer */
  pci_iomem = ioremap(mmio_start, mmio_len);
  if (!pci_iomem)
  {
    LOG_ERROR("Failed to get IO memory pointer");
    pci_release_device(pdev);
    return -EIO;
  }
  LOG_DEBUG("Mapped resource 0x%p", pci_iomem);

  /* Request IRQs */
  if(pdev->irq)
  {
    err = request_irq(pdev->irq, pci_driver_irq_handler, IRQF_SHARED, DRIVER_NAME, pdev);

    if(err)
    {
      LOG_WARNING("IRQ%d not free", pdev->irq);
      return err;
    }
  
    LOG_DEBUG("IRQ%d successfully requested", pdev->irq);
  }
  else
  {
    LOG_ERROR("No IRQ required/requested");
  }

  /* Reset SPU and queues */
  cntl_reg_1 = (1<<RESET_SPU_FLAG) | (1<<RESET_PCI_Q_FLAG)  | (1<<RESET_SPU2CPU_Q_FLAG) |
               (1<<RESET_TSC_FLAG) | (1<<RESET_SPU_IP_FLAG) | (1<<SPU2CPU_DRDY_INT_CLR) |
               (1<<SYS2SPU_QOVF_INT_CLR);
  LOG_DEBUG("Reseting SPU and queues with CNTL_REG_1 = 0x%08x to address 0x%02x", cntl_reg_1, CNTL_REG_1);
  iowrite32(cntl_reg_1, pci_iomem + (CNTL_REG_1<<ADDR_SHIFT));
  LOG_DEBUG("Reset SPU and queues");

  /* Init SPU */
  cntl_reg_0 = (1<<SPU2CPU_DRDY_INT_EN) | (1<<SYS2SPU_QOVF_INT_EN);
  LOG_DEBUG("Intalizing SPU with CNTL_REG_0 = 0x%08x to address 0x%02x", cntl_reg_0, CNTL_REG_0);
  iowrite32(cntl_reg_0, pci_iomem + (CNTL_REG_0<<ADDR_SHIFT));
  LOG_DEBUG("Initialize SPU");

  /* Get SPU current state registers */
  LOG_DEBUG("Current state is 0x%08x:0x%08x", ioread32(pci_iomem + (STATE_REG_0<<ADDR_SHIFT)),
                                              ioread32(pci_iomem + (STATE_REG_1<<ADDR_SHIFT)));

  return 0;
}

/* Function called on PCI driver unregister */
static void pci_driver_remove(struct pci_dev *pdev)
{
  /* Release maped IO memory */
  if(pci_iomem)
  {
    iounmap(pci_iomem);
  }

  pci_release_device(pdev);
}

static int read_device_config(struct pci_dev *pdev)
{
  u16 vendor, device, status_reg, command_reg;

  /* Read configuration words */
  pci_read_config_word(pdev, PCI_VENDOR_ID, &vendor);
  pci_read_config_word(pdev, PCI_DEVICE_ID, &device);
  pci_read_config_byte(pdev, PCI_REVISION_ID, &revision);
  LOG_DEBUG("Device is %04x:%04x with revision %x", vendor, device, revision);

  /* Read current status */
  pci_read_config_word(pdev, PCI_STATUS, &status_reg);
  LOG_DEBUG("Device status register (config space) is 0x%08x", status_reg);

  /* Check if device support memory access */
  pci_read_config_word(pdev, PCI_COMMAND, &command_reg);
  if (command_reg | PCI_COMMAND_MEMORY)
  {
    LOG_DEBUG("Device supports memory access");
    return 0;
  }

  LOG_ERROR("Device does not support memory access!");
  return -EIO;
}

/* Release PCI device */
static void pci_release_device(struct pci_dev *pdev)
{
  /* Free IRQs */
  if(pdev->irq)
  {
    free_irq(pdev->irq, pdev);
  }

  /* Remove mapped memory and disable device */
  pci_release_region(pdev, pci_select_bars(pdev, IORESOURCE_MEM));
  pci_disable_device(pdev);
}

/* PCI device IRQ handler */
static irqreturn_t pci_driver_irq_handler(int irq, void *pdev)
{
  LOG_DEBUG("IRQ happened");
  return 0;
}