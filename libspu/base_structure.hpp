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
#include "libspu.hpp"
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
  u32 get_power();
  status_t insert(key_t key, value_t value, flags_t flags = NO_FLAGS);
  status_t del(key_t key, flags_t flags = NO_FLAGS);
  pair_t search(key_t key, flags_t flags = P_FLAG);
  pair_t min(flags_t flags = P_FLAG);
  pair_t max(flags_t flags = P_FLAG);
  pair_t next(key_t key, flags_t flags = P_FLAG);
  pair_t prev(key_t key, flags_t flags = P_FLAG);
  pair_t nsm(key_t key, flags_t flags = P_FLAG);
  pair_t ngr(key_t key, flags_t flags = P_FLAG);
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
u32 BaseStructure::get_power()
{
  return this->power;
}

/* Insert command execution */
status_t BaseStructure::insert(key_t key, value_t value, flags_t flags)
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

/* Delete command execution */
status_t BaseStructure::del(key_t key, flags_t flags)
{
  /* Initialize INS command */
  del_cmd_t del =
  {
    .cmd  = CMD(DEL | flags),
    .gsid = gsid,
    .key  = key
  };
  del_rslt_t result;

  /* Execute del command */
  result = fops.execute<del_cmd_t, del_rslt_t>(del);

  power = result.power;

  return result.rslt;
}

/* Search command execution */
pair_t BaseStructure::search(key_t key, flags_t flags)
{
  /* Initialize INS command */
  srch_cmd_t srch =
  {
    .cmd  = CMD(SRCH | flags),
    .gsid = gsid,
    .key  = key
  };
  srch_rslt_t result;

  /* Execute srch command */
  result = fops.execute<srch_cmd_t, srch_rslt_t>(srch);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Min command execution */
pair_t BaseStructure::min(flags_t flags)
{
  /* Initialize INS command */
  min_cmd_t min =
  {
    .cmd  = CMD(MIN | flags),
    .gsid = gsid
  };
  min_rslt_t result;

  /* Execute min command */
  result = fops.execute<min_cmd_t, min_rslt_t>(min);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Max command execution */
pair_t BaseStructure::max(flags_t flags)
{
  /* Initialize INS command */
  max_cmd_t max =
  {
    .cmd  = CMD(MAX | flags),
    .gsid = gsid
  };
  max_rslt_t result;

  /* Execute max command */
  result = fops.execute<max_cmd_t, max_rslt_t>(max);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Next command execution */
pair_t BaseStructure::next(key_t key, flags_t flags)
{
  /* Initialize INS command */
  next_cmd_t next =
  {
    .cmd  = CMD(NEXT | flags),
    .gsid = gsid,
    .key = key
  };
  next_rslt_t result;

  /* Execute next command */
  result = fops.execute<next_cmd_t, next_rslt_t>(next);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Previous command execution */
pair_t BaseStructure::prev(key_t key, flags_t flags)
{
  /* Initialize INS command */
  prev_cmd_t prev =
  {
    .cmd  = CMD(PREV | flags),
    .gsid = gsid,
    .key = key
  };
  prev_rslt_t result;

  /* Execute prev command */
  result = fops.execute<prev_cmd_t, prev_rslt_t>(prev);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Next Smaler command execution */
pair_t BaseStructure::nsm(key_t key, flags_t flags)
{
  /* Initialize INS command */
  nsm_cmd_t nsm =
  {
    .cmd  = CMD(NSM | flags),
    .gsid = gsid,
    .key = key
  };
  nsm_rslt_t result;

  /* Execute nsm command */
  result = fops.execute<nsm_cmd_t, nsm_rslt_t>(nsm);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

/* Next Greater command execution */
pair_t BaseStructure::ngr(key_t key, flags_t flags)
{
  /* Initialize INS command */
  ngr_cmd_t ngr =
  {
    .cmd  = CMD(NGR | flags),
    .gsid = gsid,
    .key = key
  };
  ngr_rslt_t result;

  /* Execute ngr command */
  result = fops.execute<ngr_cmd_t, ngr_rslt_t>(ngr);

  power = result.power;

  return { result.key, result.val, result.rslt };
}

} /* namespace SPU */

#endif /* BASE_STRUCTURE_HPP */