/*
  data_container_operators.h
        - implementation of structure data_container friend operator methods 

  Copyright 2019  Dubrovin Egor <dubrovin.en@ya.ru>
                  Alex Popov <alexpopov@bmsru.ru>
                  Bauman Moscow State Technical University
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WOTHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef DATA_CONTAINER_OPERATORS_HPP
#define DATA_CONTAINER_OPERATORS_HPP

#include "spu.h"

#include <cstddef>

namespace SPU
{

/* Check if all u32 from array is equal */
bool operator== (const data_t &c1, const data_t &c2)
{
  for(u8 i=0; i<SPU_WEIGHT; i++)
  {
    if(c1[i] != c2[i])
    {
      return false;
    }
  }
  return true;
}

/* Check if any u32 from array not equal */
bool operator!= (const data_t &c1, const data_t &c2)
{
  for(u8 i=0; i<SPU_WEIGHT; i++)
  {
    if(c1[i] != c2[i])
    {
      return true;
    }
  }
  return false;
}

/* Check from head if u32 is more then other */
bool operator>  (const data_t &c1, const data_t &c2)
{
  for(u8 i=1; i<SPU_WEIGHT+1; i++)
  {
    if(c1[SPU_WEIGHT-i] > c2[SPU_WEIGHT-i])
    {
      return true;
    }
  }
  return false;
}

/* Use other operators */
bool operator>= (const data_t &c1, const data_t &c2)
{
  return (c1 > c2) | (c1 == c2);
}

/* Check from head if all u32 is less then other */
bool operator<  (const data_t &c1, const data_t &c2)
{
  for(u8 i=1; i<SPU_WEIGHT+1; i++)
  {
    if(c1[SPU_WEIGHT-i] > c2[SPU_WEIGHT-i])
    {
      return false;
    }
  }
  return true;
}

/* Use other operators */
bool operator<= (const data_t &c1, const data_t &c2)
{
  return (c1 < c2) | (c1 == c2);
}

/* Invokes operator for all array */
data_t operator& (const data_t &c1, const data_t &c2)
{
  data_t ret = {0};
  for(u8 i=0; i<SPU_WEIGHT; i++)
  {
    ret[i] = c1[i] & c2[i];
  }
  return ret;
}

/* Invokes operator for all array */
data_t operator| (const data_t &c1, const data_t &c2)
{
  data_t ret = {0};
  for(u8 i=0; i<SPU_WEIGHT; i++)
  {
    ret[i] = c1[i] | c2[i];
  }
  return ret;
}

/* Iterates and invokes shift with left part */
data_t operator<< (const data_t &cont, const u8 &shift)
{
  data_t ret = cont;
  if(shift > 0)
  {
    data_t work_cont = cont;
    u8 work_shift    = shift;
    size_t size      = sizeof(u32)*8;

    /* Shift array at all */
    u8 idx = 0;
    while(work_shift >= size)
    {
      work_cont[idx] = 0;
      idx++;
      for(u8 i=idx; i<SPU_WEIGHT; i++)
      {
        work_cont[i] = cont[i-1];
      }
      work_shift -= size;
    }

    /* Shift every part of array */
    u32 left_part = 0;
    for(u8 i=0; i<SPU_WEIGHT; i++)
    {
      ret[i] = (work_cont[i] << work_shift) | left_part;
      left_part = work_cont[i] >> (size - work_shift);
    }
  }
  return ret;
}

/* Iterates and invokes shift with left part */
data_t operator>> (const data_t &cont, const u8 &shift)
{
  data_t ret = cont;
  if(shift > 0)
  {
    data_t work_cont = cont;
    u8 work_shift    = shift;
    size_t size      = sizeof(u32)*8;

    /* Shift array at all */
    u8 idx = 1;
    while(work_shift >= size)
    {
      work_cont[SPU_WEIGHT-idx] = 0;
      idx++;
      for(u8 i=idx; i<SPU_WEIGHT+1; i++)
      {
        work_cont[SPU_WEIGHT-i] = cont[SPU_WEIGHT-i+1];
      }
      work_shift -= size;
    }

    /* Shift every part of array */
    u32 left_part = 0;
    for(u8 i=1; i<SPU_WEIGHT+1; i++)
    {
      ret[SPU_WEIGHT-i] = (work_cont[SPU_WEIGHT-i] >> work_shift) | left_part;
      left_part = work_cont[SPU_WEIGHT-i] << (size - work_shift);
    }
  }
  return ret;
}

} /* namespace SPU */

#endif /* DATA_CONTAINER_OPERATORS_HPP */