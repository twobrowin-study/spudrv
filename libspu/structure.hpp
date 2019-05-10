/*
  structure.hpp
        - definitions of structure template class with it's void specialization
        - this is the base interface class in SPU library

  Copyright 2019  Dubrovin Egor <dubrovin.en@ya.ru>
                  Alex Popov <alexpopov@bmstu.ru>
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

#include <vector>

namespace SPU
{

/***************************************
  Structure template class declaration
***************************************/

/* Template Structure class definition */
template<typename NameT = void>
class Structure
{
  struct InsertStruct
  {
    FieldsData<NameT> key_data;
    BitFlow           value;
  };
  using InsertVector = std::vector<InsertStruct>;

private:
  BaseStructure base;
  FieldsLength<NameT> key_len;

public:
  Structure(FieldsLength<NameT> key_length) : base(), key_len(key_length) {}

  Fields<NameT> keyFields()
  {
    return Fields<NameT>(key_len);
  }

  /*************************************
    Redefinitions of BaseStructure
    commands with composite key
  *************************************/

  u32 get_power()
  {
    return base.get_power();
  }

  /* Insert */
  status_t insert(BitFlow key, BitFlow value, flags_t flags = NO_FLAGS) { return base.insert(key, (value_t) value, flags); }
  status_t insert(FieldsData<NameT> key_data, BitFlow value, flags_t flags = NO_FLAGS)
  {
    Fields<NameT> key(key_len, key_data);
    return insert(key, value, flags);
  }
  status_t insert(InsertVector insert_vector, flags_t flags = NO_FLAGS)
  {
    for(auto ex : insert_vector)
    {
      status_t status = insert(ex.key_data, ex.value, flags);
      if(status != OK)
      {
        return status;
      }
    }
    return OK;
  }

  /* Delete */
  status_t del(BitFlow key, flags_t flags = NO_FLAGS) { return base.del(key, flags); }
  status_t del(FieldsData<NameT> key_data, flags_t flags = NO_FLAGS)
  {
    Fields<NameT> key(key_len, key_data);
    return del(key, flags);
  }

  /* Search */
  pair_t search(BitFlow key, flags_t flags = P_FLAG) { return base.search(key, flags); }
  pair_t search(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return search(key, flags);
  }

  /* Min and Max */
  pair_t min(flags_t flags = P_FLAG) { return base.min(flags); }
  pair_t max(flags_t flags = P_FLAG) { return base.max(flags); }

  /* Next */
  pair_t next(BitFlow key, flags_t flags = P_FLAG) { return base.next(key, flags); }
  pair_t next(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return next(key, flags);
  }

  /* Prev */
  pair_t prev(BitFlow key, flags_t flags = P_FLAG) { return base.prev(key, flags); }
  pair_t prev(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return prev(key, flags);
  }

  /* NSM */
  pair_t nsm(BitFlow key, flags_t flags = P_FLAG) { return base.nsm(key, flags); }
  pair_t nsm(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return nsm(key, flags);
  }

  /* NGR */
  pair_t ngr(BitFlow key, flags_t flags = P_FLAG) { return base.ngr(key, flags); }
  pair_t ngr(FieldsData<NameT> key_data, flags_t flags = P_FLAG)
  {
    Fields<NameT> key(key_len, key_data);
    return ngr(key, flags);
  }

};


/***************************************
  Structure special class declaration
***************************************/

/* Structure class void specialization witch is only BaseStructure son */
template<>
class Structure<void> : public BaseStructure
{
  struct InsertStruct
  {
    BitFlow key;
    BitFlow value;
  };
  using InsertVector = std::vector<InsertStruct>;

public:
  Structure() : BaseStructure() {}

  /* BaseStructure overload with BitFlow insertion */
  status_t insert ( BitFlow key, BitFlow value, flags_t flags = NO_FLAGS) { return BaseStructure::insert ( key, value, flags); }
  status_t del    ( BitFlow key, flags_t flags = NO_FLAGS)                { return BaseStructure::del    ( key, flags); }
  pair_t search   ( BitFlow key, flags_t flags = P_FLAG)                  { return BaseStructure::search ( key, flags); }
  pair_t next     ( BitFlow key, flags_t flags = P_FLAG)                  { return BaseStructure::next   ( key, flags); }
  pair_t prev     ( BitFlow key, flags_t flags = P_FLAG)                  { return BaseStructure::prev   ( key, flags); }
  pair_t nsm      ( BitFlow key, flags_t flags = P_FLAG)                  { return BaseStructure::nsm    ( key, flags); }
  pair_t ngr      ( BitFlow key, flags_t flags = P_FLAG)                  { return BaseStructure::ngr    ( key, flags); }

  /* Mass insert overload */
  status_t insert(InsertVector insert_vector, flags_t flags = NO_FLAGS)
  {
    for(auto ex : insert_vector)
    {
      status_t status = insert(ex.key, ex.value, flags);
      if(status != OK)
      {
        return status;
      }
    }
    return OK;
  }
};

} /* namespace SPU */

#endif /* STRUCTURE_HPP */