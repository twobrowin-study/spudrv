/*
  base_structure.hpp
        - base structure class declaration and implementation
        - structure is the main concept in whole SPU API
        - structure is a set of key-value pairs
        - this class defines and implements all structure methods without any key portion mechanism

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

#ifndef BASE_STRUCTURE_HPP
#define BASE_STRUCTURE_HPP

#include "spu.h"
#include "fileops.hpp"

namespace SPU
{

/***************************************
  BaseStructure class declaration
***************************************/

/* Structure in SPU */
class BaseStructure
{
private:
  gsid_t gsid = { 0 };       // Global Structure ID
  Fileops fops;              // File operations provider
  u32 power;                 // Current structure power

public:
  BaseStructure();
  ~BaseStructure();
  rslt_t insert(key_t key, value_t value, flags_t flags = NO_FLAGS);
};



/***************************************
  BaseStructure class implementation
***************************************/

/* Constructor from nothing */
BaseStructure::BaseStructure() :
  fops("/dev/" SPU_CDEV_NAME), power(0)
{
  /* Initialize ADDS command */
  adds_cmd_t adds =
  {
    .cmd = CMD(ADDS | P_FLAG)
  };
  adds_rslt_t result;

  /* Execute ADDS command */
  result = fops.execute<adds_cmd_t, adds_rslt_t>(adds);

  if(result.rslt == OK)
  {  
    /* Create GSID */
    gsid = result.gsid;
  }
}

/* Destructor witch DELS SPU structure */
BaseStructure::~BaseStructure()
{
  /* Initialize DELS command */
  dels_cmd_t dels =
  {
    .cmd  = CMD(DELS | P_FLAG),
    .gsid = gsid
  };
  dels_rslt_t result;

  /* Execute DELS command */
  result = fops.execute<dels_cmd_t, dels_rslt_t>(dels);

  power = result.power;
}

/* Insert command execution */
rslt_t BaseStructure::insert(key_t key, value_t value, flags_t flags)
{
  /* Initialize INS command */
  ins_cmd_t ins =
  {
    .cmd  = CMD(INS | flags),
    .gsid = gsid,
    .key  = key,
    .val  = value
  };
  ins_rslt_t result;

  /* Execute INS command */
  result = fops.execute<ins_cmd_t, ins_rslt_t>(ins);

  power = result.power;

  return result.rslt;
}

} /* namespace SPU */

#endif /* BASE_STRUCTURE_HPP */