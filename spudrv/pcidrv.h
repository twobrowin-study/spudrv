/*
  pcidrv.h
        - PCI driver operations definition

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

#ifndef PCIDRV_H
#define PCIDRV_H

/* Vendor and Device ID's */
#define VENDOR_ID 0x2323
#define DEVICE_ID 0x0020

/* SPU inside address shift */
#define   ADDR_SHIFT  2

/* Read/Write registers */
#define   KEY_REG  0x00
#define   VAL_REG  0x08

/* Write only registers */
#define   CMD_REG     0x10
#define   CNTL_REG_0  0x11
#define   CNTL_REG_1  0x12

/* Read only registers */
#define   POWER_REG    0x20
#define   STATE_REG_0  0x24
#define   STATE_REG_1  0x25

/* State 0 registers bits */
#define   SPU_READY_FLAG          0
#define   SPU_ERROR_FLAG          1
#define   SPU_ERROR_Q_FLAG        2
#define   DDR_Q_OVF_FLAG          3
#define   DDR_TEST_SUCC_FLAG      4
#define   SYS2SPU_Q_FULL_FLAG     5

/* State 1 registers bits */
#define   PCI2SYS_Q_EMP_FLAG      0
#define   PCI2SYS_Q_FULL_FLAG     1
#define   SPU2CPU_Q_EMP_FLAG      2
#define   SPU2CPU_Q_FULL_FLAG     3
#define   SYS2PCI_Q_EMP_FLAG      4
#define   SYS2SPU_Q_EMP_FLAG      5
#define   SPU2CPU_DRDY_INT_FLAG   6
#define   SYS2SPU_QOVF_INT_FLAG   7

/* Control 0 registers bits */
#define   ALLOW_MISD_FLAG         0
#define   SUSPEND_Q_FLAG          1
#define   LSM_DMA_FLAG            2
#define   LCM_DMA_FLAG            3
#define   ENABLE_TSC_FLAG         4
#define   SPU2CPU_DRDY_INT_EN     5
#define   SYS2SPU_QOVF_INT_EN     6

/* Control 1 registers bits */
#define   RESET_SPU_FLAG          0
#define   RESET_PCI_Q_FLAG        1
#define   RESET_SPU2CPU_Q_FLAG    2
#define   RESET_TSC_FLAG          3
#define   SHIFT_SPU2CPU_Q_FLAG    4
#define   RESET_SPU_IP_FLAG       6
#define   SPU2CPU_DRDY_INT_CLR    7
#define   SYS2SPU_QOVF_INT_CLR    8

/* SPU architecture constants  */
#define STURCTURE_NUM      3                      // Log2(Number of all structures in SPU + zero structure)
#define CMD_SHIFT(cmd)     (cmd<<3*STURCTURE_NUM) // Shift of command itself in command register
#define STR_A_SHIFT(cmd)   (cmd<<2*STURCTURE_NUM) // Shift of structure A in AND, OR, NOT, LS, LSEQ, GR, GREQ
#define STR_B_SHIFT(cmd)   (cmd<<1*STURCTURE_NUM) // Shift of structure B in AND, OR, NOT
#define STR_R_SHIFT(cmd)   (cmd<<0)               // Shift of structure R in AND, OR, NOT, LS, LSEQ, GR, GREQ

/* PCI burst write and read structure */
struct pci_burst {
  u8 count;        // Words count - 256 max
  u32 *addr_shift; // Address shift in PCI memory
  u32 *data;       // Data (data to be written or read from the device)
};

/* Create and destroy driver functions */
int create_pci_driver(void);
void destroy_pci_driver(void);

/* Interface functions */
u8 pci_get_revision(void);
void pci_single_write(u32 data, u32 addr_shift);
u32 pci_single_read(u32 addr_shift);
void pci_burst_write(const struct pci_burst *pci_burst);
void pci_burst_read(const struct pci_burst *pci_burst);

#endif /* PCIDRV_H */