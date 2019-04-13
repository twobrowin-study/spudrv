/*
  chardev.c
        - Leonhard SPU character device implementation
        - char device is placed into /dev/spu

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
#define LOG_OBJECT "character device"

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include "spu.h"
#include "log.h"
#include "info.h"
#include "chardev.h"
#include "cmdexec.h"

/* Static global vars */
static struct device* device = NULL;    // Device itself
static struct cdev char_device;         // Character device
static int cdev_major = 0;              // Device major number
static struct class* cdev_class = NULL; // Device class structure, need to interact with udev

/* Char device file operations functions definitions */
static int cdev_open(struct inode *inode, struct file *file);
static int cdev_release(struct inode *inode, struct file *file);
static ssize_t cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset);

/* Char device file operations registration */
static const struct file_operations cdev_fops =
{
  .owner   = THIS_MODULE,
  .open    = cdev_open,
  .release = cdev_release,
  .write   = cdev_write
};

/* Create character device */
int create_char_device(void)
{
  int cdev_minor = 0;
  dev_t dev;

  // Allocate mem region for the device
  if(alloc_chrdev_region(&dev, 0, 1, SPU_CDEV_NAME) != 0)
  {
    LOG_ERROR("Cannot allocate character device region");
    return -ENOMEM;
  }
  LOG_DEBUG("Character device region allocated");

  // Get the MAJOR char dev number from the kernel
  cdev_major = MAJOR(dev);
  LOG_DEBUG("Major number generated");

  // Register sysfs class
  cdev_class = class_create(THIS_MODULE, SPU_CDEV_NAME);
  LOG_DEBUG("Sysfs class registered");

  /* Init a new char device */
  cdev_init(&char_device, &cdev_fops);
  char_device.owner = THIS_MODULE;
  LOG_DEBUG("Character device initialized");

  // Add char dev into kernel
  cdev_add(&char_device, MKDEV(cdev_major, cdev_minor), 1);
  LOG_DEBUG("Character device added into kernel");

  // Creating character device file itself
  device = device_create(cdev_class, NULL, MKDEV(cdev_major, cdev_minor), NULL, SPU_CDEV_NAME);
  LOG_DEBUG("Device created");

  return 0;
}

/* Destroy character device */
void destroy_char_device(void)
{
  int cdev_minor = 0;

  /* Undo actions from create_char_device */
  device_destroy(cdev_class, MKDEV(cdev_major, cdev_minor));
  class_unregister(cdev_class);
  class_destroy(cdev_class);
  unregister_chrdev_region(MKDEV(cdev_major, cdev_minor), MINORMASK);
}

/* Function called on file open */
static int cdev_open(struct inode *inode, struct file *file)
{
  LOG_DEBUG("Character device opened");
  return 0;
}

/* Function called on file close */
static int cdev_release(struct inode *inode, struct file *file)
{
  LOG_DEBUG("Character device closed");
  return 0;
}

/* Function called on write */
static ssize_t cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *offset)
{
  void *usr_buf = (void*) buf;
  void *usr_cmd = kmalloc(count, GFP_KERNEL);
  const void *usr_res = NULL;
  int rslt_count = 0;

  LOG_DEBUG("Character device write operation invoked");

  /* Copy command struct from user space */
  if(copy_from_user(usr_cmd, usr_buf, count))
  {
    LOG_ERROR("Character device could not copy data from user space");
    return -EFAULT;
  }
  LOG_DEBUG("Character device copy command from user");

  LOG_DEBUG("Character device gave command to execute");
  rslt_count = execute_cmd(usr_cmd, &usr_res);

  /* Check if result has length */
  if(rslt_count > 0)
  {
    LOG_DEBUG("Character device got command result");
  
    /* Copy result struct into user space */
    if(copy_to_user(usr_buf, usr_res, rslt_count))
    {
      LOG_ERROR("Character device could not copy data into user space");
      return -EFAULT;
    }
    LOG_DEBUG("Character device wrote result to user");

    if(usr_cmd)
    {
      kzfree(usr_cmd);
      LOG_DEBUG("Delete command container");
    }

    if(usr_res)
    {
      kzfree(usr_res);
      LOG_DEBUG("Delete result container");
    }
  }
  else
  {
    LOG_ERROR("Character device got no result of an operation");
  }

  return rslt_count;
}