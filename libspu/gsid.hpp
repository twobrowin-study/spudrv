/*
  gsid.hpp
        - GSID provider class declaration and implementation

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

#ifndef GSID_HPP
#define GSID_HPP

#include <string>

#include "spu.h"

namespace SPU
{

/***************************************
  GSID class declaration
***************************************/

/* Global Structure ID */
class GSID
{
private:
  u32 gsid[GSID_WEIGHT] = {0}; // Global GSID ID

public:
  /* Creator */
  void create(u32* gsid)
  {
    /* Copy GSID to object */
    std::copy(gsid,gsid + GSID_WEIGHT, this->gsid);
  }

  /* Convert into string by formatting */
  std::string to_std_string()
  {
    char buff[100];
    snprintf(buff, sizeof(buff), GSID_FORMAT, GSID_VAR(gsid));
    return std::string(buff);
  }
};

} /* namespace SPU */

#endif /* GSID_HPP */