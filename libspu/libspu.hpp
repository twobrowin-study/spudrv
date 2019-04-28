/*
  libspu.hpp
        - SPU library to include file
        - simultaneously c++ and pure c header
        - file defines containers for key and value data and some more type hiders

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

#ifndef LIBSPU_HPP
#define LIBSPU_HPP

#include "spu.h"
#include "fields.hpp"
#include "structure.hpp"

#include <string>

namespace SPU
{

/* Convert GSID to string */
std::string to_string(gsid_t gsid)
{
  char buf[100];
  snprintf(buf, sizeof(buf), GSID_FORMAT, GSID_VAR(gsid));
  return std::string(buf);
}

/* Convert data container to string */
std::string to_string(struct data_container data)
{
  std::string ret;
  for (u32 cont : data.cont)
  {
    char buf[100];
    snprintf(buf, sizeof(buf), "0x%08x-", cont);
    ret += buf;
  }
  return ret.substr(0, ret.size()-1);
}

/* Convert result into string */
std::string to_string(status_t status)
{
  /* Check if status is OK */
  if(status == OK)
  {
    return "OK";
  }

  std::string ret;

  /* If status is not OK find all parts */
  if(status & ERR)
  {
    ret += "ERR & ";
  }

  if(status & QERR)
  {
    ret += "QERR & ";
  }

  if(status & OERR)
  {
    ret += "OERR & ";
  }

  return ret.substr(0, ret.size()-3);
}

/* Convert key : value pair with status into string */
std::string to_string(pair_t pair)
{
  return to_string(pair.status) + " : " + to_string(pair.key) + " : " + to_string(pair.value);
}

} /* namespace SPU */

#endif /* LIBSPU_HPP */