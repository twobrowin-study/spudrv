/*
  structure.hpp
        - definitions of structure template class with it's void specialization
        - this is the base interface class in SPU library

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

#include "libspu.hpp"
#include "fields.hpp"
#include "base_structure.hpp"

namespace SPU
{

/***************************************
  Structure template class declaration
***************************************/

/* Template Structure class definition */
template<typename NameT = void>
class Structure
{
private:
  BaseStructure base;
  FieldsLength<NameT> key_len;

public:
  Structure(FieldsLength<NameT> key_length) : base(), key_len(key_length) {}

  /*************************************
    Redefinitions of BaseStructure
    commands with composite key
  *************************************/

  u32 get_power()
  {
    return base.get_power();
  }

  status_t insert(FieldsData<NameT> key_data, value_t value, flags_t flags = NO_FLAGS)
  {
    Fields<NameT> key(key_len, key_data);
    return base.insert(key, value, flags);
  }

  status_t del(FieldsData<NameT> key_data, flags_t flags = NO_FLAGS)
  {
    Fields<NameT> key(key_len, key_data);
    return base.del(key, flags);
  }

  pair_t search(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return base.search(key, flags);
  }

  pair_t min(flags_t flags = P_FLAG)
  {
    return base.min(flags);
  }

  pair_t max(flags_t flags = P_FLAG)
  {
    return base.max(flags);
  }

  pair_t next(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return base.next(key, flags);
  }

  pair_t prev(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return base.prev(key, flags);
  }

  pair_t nsm(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return base.nsm(key, flags);
  }

  pair_t ngr(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return base.ngr(key, flags);
  }

};


/***************************************
  Structure special class declaration
***************************************/

/* Structure class void specialization witch is only BaseStructure son */
template<>
class Structure<void> : public BaseStructure
{
public:
  Structure() : BaseStructure() {}
};

} /* namespace SPU */

#endif /* STRUCTURE_HPP */