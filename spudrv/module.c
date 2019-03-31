/*
  module.c
        - PCI Driver (GNU/Linux kernel module) for Leonhard SPU v04 - 64 bits, 100M key-value pairs, Baikal-T1 support
        - driver is built to run with libspu - an open API for Leonhard SPU

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
#define LOG_OBJECT "module"

#include <linux/module.h>
#include <linux/init.h>

#include "spu.h"
#include "log.h"
#include "info.h"
#include "module.h"
#include "pcidrv.h"
#include "chardev.h"

/* Module about information */
MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);

/* Module init and exit functions */
static int __init module_init_func(void);
static void __exit module_exit_func(void);

/* Module init and exit registration */
module_init(module_init_func);
module_exit(module_exit_func);

/* Function called on module initialization (loading to kernel) */
static int __init module_init_func(void)
{
  int err;

  LOG_INFO("Loading %s - version %s", DRIVER_DESCRIPTION, DRIVER_VERSION);
  LOG_INFO("%s", DRIVER_COPYRIGHT);

  /* Create PCI driver */
  err = create_pci_driver();
  if(err)
  {
    LOG_ERROR("PCI driver create fault");
    return err;
  }
  LOG_DEBUG("PCI driver created");

  /* Create character device */
  err = create_char_device();
  if(err)
  {
    LOG_ERROR("Character device create fault");
    return err;
  }
  LOG_DEBUG("Character device created");

  LOG_INFO("Module load success");
  return 0;
}

/* Function called on module exit (removing from kernel) */
static void __exit module_exit_func(void)
{
  LOG_DEBUG("Module remove");

  destroy_pci_driver();
  LOG_DEBUG("PCI driver destroyed");
 
  destroy_char_device();
  LOG_DEBUG("Character device destroyed");

  LOG_INFO("%s removed", DRIVER_DESCRIPTION);
}