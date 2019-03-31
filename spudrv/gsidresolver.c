/*
  gsidresolver.c
        - container of structures by GSIDs
        - copy unused structures into PC's RAM

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
#define LOG_OBJECT "GSID resolver"

#include <linux/slab.h>
#include <linux/random.h>

#include "spu.h"
#include "log.h"
#include "info.h"
#include "pcidrv.h"
#include "gsidresolver.h"

// Structures GSID's currently in SPU memory - init with 0 GSID's
static u32 gsids_in_spu[SPU_STR_NUM][GSID_WEIGHT] = { 0 };

/* Create new GSID -> generate it and add into memory */
int create_gsid(u32 gsid[GSID_WEIGHT])
{
  u8 i;

  /* Only GSID_WEIGHT = 4 supports - in other cases result is 0 */
#if GSID_WEIGHT == 4

  /* Generating GSID */

  // First 32 bits - is current driver version and SPU revision
  gsid[0] = (DRIVER_VERSION_NUM<<16) | (pci_get_revision());

  /* Second and third 32 bits - just random */
  gsid[1] = get_random_int();
  gsid[2] = get_random_int();
 
  // Last 32 bits is time in ms
  gsid[3] = get_seconds();

#endif /* GSID_WEIGHT == 4 */

  LOG_DEBUG("Generated GSID" GSID_FORMAT, GSID_VAR(gsid));

  /* Add GSID into GSID container */

  /* Try to add GSID into SPU local memory */
  for(i=0; i<SPU_STR_NUM; i++)
  {
    if(*gsids_in_spu[i] == 0) // Check if GSID is zero (actually just first 32 bits)
    {
      memcpy(gsids_in_spu[i], gsid, GSID_WEIGHT*sizeof(u32));
      LOG_DEBUG("Add GSID:" GSID_FORMAT "to SPU memory position %d", GSID_VAR(gsids_in_spu[i]), SPU_STR(i));
      return 0;
    }
  }

  // Try was unsuccess
  LOG_ERROR("No space in SPU memory for GSID:" GSID_FORMAT, GSID_VAR(gsid));
  return -ENOKEY;
}

/* Get structure number from local SPU memory by GSID */
int resolve_gsid(const u32 gsid[GSID_WEIGHT], u8 cmd)
{
  u8 i, j;

  /* Try to find GSID in SPU local memory */
  for(i=0; i<SPU_STR_NUM; i++)
  {
    if(GSID_EQUAL(gsid, gsids_in_spu[i]))
    {
      LOG_DEBUG("Found GSID:" GSID_FORMAT "at SPU memory position %d", GSID_VAR(gsids_in_spu[i]), SPU_STR(i));
      break;
    }
  }

  /* Try was unsuccess */
  if (i == SPU_STR_NUM)
  {
    LOG_DEBUG("Did not found GSID" GSID_FORMAT "in SPU memory", GSID_VAR(gsid));
    return -ENOKEY;
  }

  /* In case commad is delete structure - deleting GSID from memory */
  if(cmd == DELS)
  {
    /* Clear this GSID */
    for(j=0; j<GSID_WEIGHT; j++)
    {
      gsids_in_spu[i][j] = 0;
    }
    LOG_DEBUG("Delete GSID" GSID_FORMAT "from SPU memory", GSID_VAR(gsid));
  }

  return SPU_STR(i);
}