/*
  cmdexec.c
        - SPU commands executors

  Copyright 2019  Dubrovin Egor <dubrovin.en@ya.ru>
                  Alex Popov <alexpopov@bmstu.ru>
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
#define LOG_OBJECT "command execution"

#include <linux/slab.h>
#include <linux/delay.h>

#include "spu.h"
#include "log.h"
#include "pcidrv.h"
#include "cmdexec.h"
#include "gsidresolver.h"

/* Internal functions */
static size_t alloc_rslt(const void **res_buf, u8 cmd);
static void adds(const void *res_buf);
static int init_burst_w(struct pci_burst *pci_burst, u8 cmd, const void *cmd_buf);
static int init_burst_r(struct pci_burst *pci_burst, u8 cmd);
static int poll_spu(u8 reg, u8 shift, u8 *state);
static void set_rsltfrmt(struct pci_burst *pci_burst, u8 cmd, const void *res_buf, u8 spu_status);

/* Commands execution in command workflow */
size_t execute_cmd(const void *cmd_buf, const void **res_buf)
{
  u8 spu_state = 0, spu_status;
  size_t rslt_size = 0;
  
  struct pci_burst pci_burst_w =
  {
    .count      = 0,
    .addr_shift = NULL,
    .data       = NULL
  };

  struct pci_burst pci_burst_r =
  {
    .count      = 0,
    .addr_shift = NULL,
    .data       = NULL
  };

  /* Set up command number from format 0 */
  u8 cmd = CMDFRMT_0(cmd_buf)->cmd;
  LOG_DEBUG("Executing command 0x%02x with Q=%d, R=%d, P=%d", PURE_CMD(cmd), GET_Q_FLAG(cmd), GET_R_FLAG(cmd), GET_P_FLAG(cmd)); 

  /* Allocate result structure with pulling */
  rslt_size = alloc_rslt(res_buf, cmd);

  /* Special case ADDS command - no PCI transactions need */
  if(PURE_CMD(cmd) == ADDS)
  {
    adds(*res_buf);
    return rslt_size;
  }

  /* Init to-write burst structure */
  if(init_burst_w(&pci_burst_w, cmd, cmd_buf) != 0)
  {
    LOG_ERROR("Could not initialize to-write burst structure");
    return -ENOMEM;
  }  

  /* Init burst to-read structure */
  if(init_burst_r(&pci_burst_r, cmd) != 0)
  {
    LOG_ERROR("Could not initialize burst to-read structure");
    return -ENOMEM;
  }
  LOG_DEBUG("PCI burst structures initialized");

  /* Poll SPU queue ready flag if queuing and no queue reset */
  if((GET_Q_FLAG(cmd) == 1) && (GET_R_FLAG(cmd) == 0))
  {
    LOG_DEBUG("Polling SPU queue ready state");
    if(poll_spu(STATE_REG_1, SYS2SPU_Q_EMP_FLAG, &spu_state) != 0)
    {
      LOG_ERROR("SPU queue is not ready for operation");
      return -ENOEXEC;
    }
    LOG_DEBUG("SPU queue is ready for operation");
  }

  /* Poll SPU ready for next operation */
  if(poll_spu(STATE_REG_0, SPU_READY_FLAG, &spu_state) != 0)
  {
    LOG_ERROR("SPU is not ready for operation");
    return -ENOEXEC;
  }
  LOG_DEBUG("SPU is ready for operation");

  /* Execute command */
  LOG_DEBUG("Starting operation execution");
  pci_burst_write(&pci_burst_w);

  /* Poll execution end */
  if(GET_P_FLAG(cmd) == 1)
  {
    LOG_DEBUG("Polling operation finish");
    spu_status = 0;
    if(poll_spu(STATE_REG_0, SPU_READY_FLAG, &spu_status) != 0)
    {
      LOG_ERROR("SPU can not finish operation");
      return -ENOEXEC;
    }
    LOG_DEBUG("SPU finish operation");

    /* Read results */
    pci_burst_read(&pci_burst_r);
    set_rsltfrmt(&pci_burst_r, cmd, *res_buf, spu_status);
    LOG_DEBUG("Got results of operation");
  }
  else
  {
    LOG_DEBUG("Would not poll operation end");
  }

  /* Kill burst structures */
  if(pci_burst_w.addr_shift)
  {
    kfree(pci_burst_w.addr_shift);
  }
  if(pci_burst_w.data)
  {
    kfree(pci_burst_w.data);
  }
  if(pci_burst_r.addr_shift)
  {
    kfree(pci_burst_r.addr_shift);
  }
  if(pci_burst_r.data)
  {
    kfree(pci_burst_r.data);
  }
  LOG_DEBUG("PCI burst structures deleted");

  /* Return */
  return rslt_size;
}

/* Allocate result structure */
static size_t alloc_rslt(const void **res_buf, u8 cmd)
{
  size_t rslt_size = 0;

  if(GET_P_FLAG(cmd) == 0)
  {
    LOG_DEBUG("Allocate result format 0 structure because no polling need");
    rslt_size = sizeof(struct rsltfrmt_0);
  }
  else
  {
    /* Set result size for format */
    switch(PURE_CMD(cmd))
    {
      CASE_RSLTFRMT_0:
        LOG_DEBUG("Allocate result format 0 structure");
        rslt_size = sizeof(struct rsltfrmt_0);
        break;

      CASE_RSLTFRMT_1:
        LOG_DEBUG("Allocate result format 1 structure");
        rslt_size = sizeof(struct rsltfrmt_1);
        break;

      CASE_RSLTFRMT_2:
        LOG_DEBUG("Allocate result format 2 structure");
        rslt_size = sizeof(struct rsltfrmt_2);
        break;

      default:
        LOG_ERROR("Command was not found to allocate result");
        return -ENOEXEC;
    }
  }

  /* Allocate */
  *res_buf = kmalloc(rslt_size, GFP_KERNEL);
  if(!(*res_buf))
  {
    LOG_ERROR("Could not allocate result structure");
    return -ENOMEM;
  }
  LOG_DEBUG("Allocate result with size %ld", (unsigned long int)rslt_size);

  /* Set standard error return code (if no polling required that wold be OK) */
  RSLTFRMT_0(*res_buf)->rslt = GET_P_FLAG(cmd) ? ERR : OK;

  return rslt_size;
}

/* ADDS command executor */
static void adds(const void *res_buf)
{
  LOG_DEBUG("ADDS command execution");

  /* Result generation */
  if(create_gsid(&RSLTFRMT_0(res_buf)->gsid) != 0)
  {
    LOG_ERROR("ADDS command execution error");
    return; // ERR result code already in result structure
  }
  RSLTFRMT_0(res_buf)->rslt = OK;

  LOG_DEBUG("ADDS return result");
}

/* Initialize burst to-write structure */
static int init_burst_w(struct pci_burst *pci_burst, u8 cmd, const void *cmd_buf)
{
  u8 count = 0;
  int str, str_a, str_b, str_r;
  u8 i;

  /* Find right burst to-write format */
  switch(PURE_CMD(cmd))
  {
    CASE_CMDFRMT_1:
      LOG_DEBUG("Allocate to-write burst structure for command format 1");

      /* Get structure number in SPU and create execution possibility */
      str = resolve_gsid(CMDFRMT_1(cmd_buf)->gsid, cmd);
      if(str <= 0)
      {
        LOG_ERROR("GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_1(cmd_buf)->gsid));
        return -ENOKEY;
      }

      /* Burst count formula: key + val + cmd/str */
      count = SPU_WEIGHT*2 + 1;
      break;

    CASE_CMDFRMT_2:
      LOG_DEBUG("Allocate to-write burst structure for command format 2");

      /* Get structure number in SPU and create execution possibility */
      str = resolve_gsid(CMDFRMT_2(cmd_buf)->gsid, cmd);
      if(str <= 0)
      {
        LOG_ERROR("GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_2(cmd_buf)->gsid));
        return -ENOKEY;
      }

      /* Burst count formula: key + cmd/str */
      count = SPU_WEIGHT + 1;
      break;

    CASE_CMDFRMT_3:
      LOG_DEBUG("Allocate to-write burst structure for command format 3");

      /* Get structure number in SPU and create execution possibility */
      str = resolve_gsid(CMDFRMT_3(cmd_buf)->gsid, cmd);
      if(str <= 0)
      {
        LOG_ERROR("GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_3(cmd_buf)->gsid));
        return -ENOKEY;
      }

      /* Burst count formula: cmd/str */
      count = 1;
      break;

    CASE_CMDFRMT_4:
      LOG_DEBUG("Allocate to-write burst structure for command format 4");

      /* Get structure number in SPU and create execution possibility */
      str_a = resolve_gsid(CMDFRMT_4(cmd_buf)->gsid_a, cmd);
      if(str_a <= 0)
      {
        LOG_ERROR("A GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_4(cmd_buf)->gsid_a));
        return -ENOKEY;
      }
      str_b = resolve_gsid(CMDFRMT_4(cmd_buf)->gsid_b, cmd);
      if(str_b <= 0)
      {
        LOG_ERROR("B GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_4(cmd_buf)->gsid_b));
        return -ENOKEY;
      }
      str_r = resolve_gsid(CMDFRMT_4(cmd_buf)->gsid_r, cmd);
      if(str_r <= 0)
      {
        LOG_ERROR("R GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_4(cmd_buf)->gsid_r));
        return -ENOKEY;
      }

      /* Burst count formula: cmd/str */
      count = 1;
      break;

    CASE_CMDFRMT_5:
      LOG_DEBUG("Allocate to-write burst structure for command format 5");

      /* Get structure number in SPU and create execution possibility */
      str_a = resolve_gsid(CMDFRMT_5(cmd_buf)->gsid_a, cmd);
      if(str_a <= 0)
      {
        LOG_ERROR("GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_5(cmd_buf)->gsid_a));
        return -ENOKEY;
      }
      str_r = resolve_gsid(CMDFRMT_5(cmd_buf)->gsid_r, cmd);
      if(str_r <= 0)
      {
        LOG_ERROR("GSID" GSID_FORMAT "was not found", GSID_VAR(CMDFRMT_5(cmd_buf)->gsid_r));
        return -ENOKEY;
      }

      /* Burst count formula: cmd/str */
      count = 1;
      break;

    default:
      LOG_ERROR("Command was not found to allocate burst to-write structure");
      return -ENOEXEC;
  }

  /* Allocate to-write burst structure */
  pci_burst->count      = count;
  pci_burst->addr_shift = kmalloc(count*sizeof(u32), GFP_KERNEL);
  pci_burst->data       = kmalloc(count*sizeof(u32), GFP_KERNEL);

  if(!pci_burst->addr_shift || !pci_burst->data)
  {
    LOG_ERROR("Could not allocate to-write burst structure");
    return -ENOMEM;
  }
  LOG_DEBUG("Burst to-write structure allocated");

  /* Initialize burst to-write structure */
  switch(PURE_CMD(cmd))
  {
    CASE_CMDFRMT_1:
      LOG_DEBUG("Initialize to-write burst structure for command format 1");

      /* Initialize in loop */
      for(i=0; i<SPU_WEIGHT; i++)
      {
        /* Set key */
        pci_burst->addr_shift[i] = KEY_REG + i;
        pci_burst->data[i]       = CMDFRMT_1(cmd_buf)->key.cont[i];

        /* Set value */
        pci_burst->addr_shift[i+SPU_WEIGHT] = VAL_REG + i;
        pci_burst->data[i+SPU_WEIGHT]       = CMDFRMT_1(cmd_buf)->val.cont[i];
      }

      /* Last one is a command */
      pci_burst->addr_shift[count-1] = CMD_REG;
      pci_burst->data[count-1]       = CMD_SHIFT( SPU_CMD( CMDFRMT_1(cmd_buf)->cmd ) ) | str;

      break;

    CASE_CMDFRMT_2:
      LOG_DEBUG("Initialize to-write burst structure for command format 2");

      /* Initialize in loop */
      for(i=0; i<SPU_WEIGHT; i++)
      {
        /* Set key */
        pci_burst->addr_shift[i] = KEY_REG + i;
        pci_burst->data[i]       = CMDFRMT_2(cmd_buf)->key.cont[i];
      }

      /* Last one is a command */
      pci_burst->addr_shift[count-1] = CMD_REG;
      pci_burst->data[count-1]       = CMD_SHIFT( SPU_CMD( CMDFRMT_2(cmd_buf)->cmd ) ) | str;

      break;

    CASE_CMDFRMT_3:
      LOG_DEBUG("Initialize to-write burst structure for command format 3");

      /* Last just a command */
      pci_burst->addr_shift[0] = CMD_REG;
      pci_burst->data[0]       = CMD_SHIFT( SPU_CMD( CMDFRMT_3(cmd_buf)->cmd ) ) | str;

      break;

    CASE_CMDFRMT_4:
      LOG_DEBUG("Initialize to-write burst structure for command format 4");

      /* Last just a command */
      pci_burst->addr_shift[0] = CMD_REG;
      pci_burst->data[0]       = CMD_SHIFT( SPU_CMD( CMDFRMT_4(cmd_buf)->cmd ) ) |
                                 STR_A_SHIFT(str_a) |
                                 STR_B_SHIFT(str_b) |
                                 STR_R_SHIFT(str_r);

      break;

    CASE_CMDFRMT_5:
      LOG_DEBUG("Initialize to-write burst structure for command format 5");

      /* Last just a command */
      pci_burst->addr_shift[0] = CMD_REG;
      pci_burst->data[0]       = CMD_SHIFT( SPU_CMD( CMDFRMT_5(cmd_buf)->cmd ) ) |
                                 STR_A_SHIFT(str_a) |
                                 STR_R_SHIFT(str_r);

      break;

    default:
      LOG_ERROR("Could not initialize burst to-write structure");
      return -ENOMEM;
  }

  return 0;
}

/* Initialize burst to-read structure */
static int init_burst_r(struct pci_burst *pci_burst, u8 cmd)
{
  u8 count = 0;
  u8 i;

  if(GET_P_FLAG(cmd) == 0)
  {
    LOG_DEBUG("Would not allocate burst to-read structure because no polling need");
    return 0;
  }

  /* Find right burst to-read format */
  switch(PURE_CMD(cmd))
  {
    CASE_RSLTFRMT_1:
      LOG_DEBUG("Allocate to-read burst structure for result format 1");

      /* Burst count formula: power */
      count = 1;
      break;

    CASE_RSLTFRMT_2:
      LOG_DEBUG("Allocate to-read burst structure for result format 2");

      /* Burst count formula: key + val + power */
      count = SPU_WEIGHT*2 + 1;
      break;

    default:
      LOG_ERROR("Could not allocate burst to-read structure");
      return -ENOEXEC;
  }

  /* Allocate burst to-read structure */
  pci_burst->count      = count;
  pci_burst->addr_shift = kmalloc(count*sizeof(u32), GFP_KERNEL);
  pci_burst->data       = kmalloc(count*sizeof(u32), GFP_KERNEL);

  if(!pci_burst->addr_shift || !pci_burst->data)
  {
    LOG_ERROR("Could not allocate burst to-read structure");
    return -ENOMEM;
  }
  LOG_DEBUG("Allocate burst to-read structure ");

  /* Initialize burst to-read structure */
  switch(PURE_CMD(cmd))
  {
    CASE_RSLTFRMT_1:
      LOG_DEBUG("Init burst to-read structure for result format 1");

      /* Just a power */
      pci_burst->addr_shift[0] = POWER_REG;
      break;


    CASE_RSLTFRMT_2:
      LOG_DEBUG("Init burst to-read structure for result format 2");

      /* Initialize in loop key and value addresses */
      for(i=0; i<SPU_WEIGHT; i++)
      {
        pci_burst->addr_shift[i]            = KEY_REG + i;
        pci_burst->addr_shift[i+SPU_WEIGHT] = VAL_REG + i;
      }

      /* Last one is a power */
      pci_burst->addr_shift[count-1] = POWER_REG;
      break;

    default:
      LOG_ERROR("Could not initialize burst to-read structure");
      return -ENOMEM;
  }

  return 0;
}

/* Poll untill SPU is ready or there is no more attempts */
static inline int poll_spu(u8 reg, u8 shift, u8 *state)
{
  u8 poll_attempts = 255;
  do
  {
    msleep(1);
    if(poll_attempts == 0)
    {
      // Error return
      return -ENOEXEC;
    }
    poll_attempts--;

    *state = pci_status_read(reg); // Get current state for result
  }
  while ( SPU_FLAG(*state, shift) == 0 );

  // Success
  return 0;
}

/* Set result output format */
static void set_rsltfrmt(struct pci_burst *pci_burst, u8 cmd, const void *res_buf, u8 spu_status)
{
  u8 count = pci_burst->count;
  u8 i;

  if(GET_P_FLAG(cmd) == 0)
  {
    LOG_DEBUG("Would not set any result format because no polling need");
    return;
  }

  /* Set result format for correct form */
  switch(PURE_CMD(cmd))
  {
    CASE_RSLTFRMT_1:
      LOG_DEBUG("Set result for format 1 with status 0x%02x", ERRORS(spu_status));
      RSLTFRMT_1(res_buf)->rslt = ERRORS(spu_status);

      /* Just a power */
      RSLTFRMT_1(res_buf)->power = pci_burst->data[count-1];
      return;

    CASE_RSLTFRMT_2:
      LOG_DEBUG("Set result for format 2 with status 0x%02x", ERRORS(spu_status));
      RSLTFRMT_2(res_buf)->rslt = ERRORS(spu_status);

      /* Initialize in loop key and value */
      for(i=0; i<SPU_WEIGHT; i++)
      {
        RSLTFRMT_2(res_buf)->key.cont[i] = pci_burst->data[i];
        RSLTFRMT_2(res_buf)->val.cont[i] = pci_burst->data[i+SPU_WEIGHT];
      }

      /* Last one is a power */
      RSLTFRMT_2(res_buf)->power = pci_burst->data[count-1];
      return;

    default:
      LOG_ERROR("Could not set result");
      return;
  }
}