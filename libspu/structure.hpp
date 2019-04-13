/*
  structure.hpp
        - structure class declaration and implementation (because it is a template)
        - structure is the main concept in whole SPU API
        - structure is a set of key-value pairs

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

#ifndef STRUCTURE_HPP
#define STRUCTURE_HPP

#include <string>
#include <sys/types.h>
#include <linux/hdreg.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>

#include "key_field.hpp"
#include "spu.h"
#include "key.hpp"
#include "gsid.hpp"

namespace SPU
{

/***************************************
  Structure template class declaration
***************************************/

/* Structure in SPU */
template<typename KeyFieldNameType> // Type to define a field name in associate arrays
class Structure
{
private:
  GSID gsid;                          // Global Structure ID
  Key<KeyFieldNameType> key_provider; // Key Provider Object

  /* File operations provider structure */
  struct
  {
    int dfd = 0; // Driver File Descriptor to connect SPU
    void *buf;   // Buffer
    size_t size; // Size of current buffer
  } fop;

  void* spu_exec(void *cmd, size_t size);

public:
  Structure(FieldLengthVector<KeyFieldNameType> fields_length_vector);
  ~Structure();
  // bool insert();
};



/***************************************
  Structure class implementation
***************************************/

/* Execute the command */
template<typename KeyFieldNameType>
void* Structure<KeyFieldNameType>::spu_exec(void *cmd, size_t size)
{
  fop.buf = cmd;
  fop.size = size;
  fop.size = write(fop.dfd, fop.buf, fop.size);
  return fop.buf;
}

/* Constructor from nothing */
template<typename KeyFieldNameType>
Structure<KeyFieldNameType>::Structure(FieldLengthVector<KeyFieldNameType> fields_length_vector) : key_provider(fields_length_vector)
{
  /* Get char dev descriptor */
  fop.dfd = open("/dev/" SPU_CDEV_NAME, O_RDWR);

  /* Init ADDS command */
  struct cmdfrmt_0 adds =
  {
    .cmd = ADDS | P_FLAG
  };

  /* Execute command */
  struct rsltfrmt_0 result_format = *( (struct rsltfrmt_0*) spu_exec(&adds, sizeof(adds)) );

  if(result_format.rslt == OK)
  {  
    /* Create GSID */
    gsid.create(result_format.gsid);
    std::cout << "Got GSID " << gsid.to_std_string() << std::endl;
  }
  else
  {
    std::cout << "ERR" << std::endl;
  }
}

/* Destructor */
template<typename KeyFieldNameType>
Structure<KeyFieldNameType>::~Structure()
{
  if (fop.dfd)
  {
    close(fop.dfd);
  }
}

} /* namespace SPU */

#endif /* STRUCTURE_HPP */