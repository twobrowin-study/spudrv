/*
  key_field.hpp
        - declaration for Key class necessaryry types

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

#ifndef KEY_FIELD_HPP
#define KEY_FIELD_HPP

#include "spu.h"

#include <vector>

namespace SPU
{

/* Container for fields length definitions */
template <typename NameType>
struct FieldLength
{
  NameType name;
  u8 length; // Max length for one field is 255
};

/* Vector for field length definitions */
template <typename NameType>
using FieldLengthVector = std::vector < FieldLength<NameType> >;

/* Container for fields data definitions */
template <typename NameType>
struct FieldData
{
  NameType name;
  u32 data; // Max data for one field is 32 bits
};

/* Vector for fields data definitions */
template <typename NameType>
using FieldDataVector = std::vector < FieldData<NameType> >;

} /* namespace SPU */

#endif /* KEY_FIELD_HPP */