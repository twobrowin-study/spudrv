/*
  log.h
        - simple logger

  Copyright 2019  Dubrovin Egor <dubrovin.en@ya.ru>
                  Alex Popov <alexpopov@bmsru.ru>
                  Bauman Moscow State Technical University

  Copyright 2017  Oleg Kutkov <elenbert@gmail.com>
                  Crimean astrophysical observatory
  
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

#ifndef LOG_H
#define LOG_H

#include <linux/kernel.h>
#include "info.h"

/* Info log message */
#ifndef LOG_INFO
#define LOG_INFO(fmt, args...) \
  printk(DRIVER_NAME " [INFO]: " fmt "\n", ## args)
#endif /* LOG_INFO */

/* Error log message */
#ifndef LOG_ERROR
#define LOG_ERROR(fmt, args...) \
  printk(DRIVER_NAME " [ERROR at " LOG_OBJECT "]: " fmt "\n", ## args)
#endif /* LOG_ERROR */

/* Error log message */
#ifndef LOG_WARNING
#define LOG_WARNING(fmt, args...) \
  printk(DRIVER_NAME " [WARNING at " LOG_OBJECT "]: " fmt "\n", ## args)
#endif /* LOG_WARNING */

/* Debug log message, witch works only when debug enabled */
#ifndef LOG_DEBUG
#ifdef DEBUG
#define LOG_DEBUG(fmt, args...) \
  printk(DRIVER_NAME " [DEBUG at " LOG_OBJECT "]: " fmt "\n", ## args)
#else /* DEBUG */
#define LOG_DEBUG(fmt, args...) \
  ((void) 0)
#endif /* DEBUG */
#endif /* LOG_DEBUG */

/* Macro to print structure GSID */
/* Only GSID_WEIGHT = 4 supports */
#if GSID_WEIGHT == 4
  #define GSID_FORMAT " %08x-%08x-%08x-%08x "
  #define GSID_VAR(var) \
    var[0], var[1], var[2], var[3]
#endif

#endif /* LOG_H */