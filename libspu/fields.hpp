/*
  fields.hpp
        - declaration of class necessary for data_t fields split

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

#ifndef FIELDS_HPP
#define FIELDS_HPP

#include "libspu.hpp"
#include "fields_containers.hpp"

namespace SPU
{

/* Fields splited data_t */
template <typename NameT = u8>
class Fields
{
  using DataVector = typename FieldsData<NameT>::DataVector;

private:
  FieldsLength<NameT> length;
  FieldsData<NameT> data;

  /* Pure data initializer */
  void init_data()
  {
    data = FieldsData<NameT>();
    for(auto ex : length.cont_vec)
    {
      data.cont_vec.push_back({ex.name, BitFlow()});
    }
  }

  /* Data initializer from data_t */
  void init_data(data_t fields_data)
  {
    data = FieldsData<NameT>();
    u8 shift = 0;
    for(auto ex : length.cont_vec)
    {
      data_t mask = length.mask(ex.cont);
      BitFlow flow( (fields_data >> shift) & mask );
      data.cont_vec.push_back({ex.name, flow});
      shift += ex.cont;
    }
  }

public:
  /* Constructors */
  Fields(FieldsLength<NameT> fields_length) : length(fields_length), data() { init_data(); }
  Fields(FieldsLength<NameT> fields_length, FieldsData<NameT> fields_data) : length(fields_length), data(fields_data) {}
  Fields(FieldsLength<NameT> fields_length, BitFlow fields_data) : length(fields_length), data() { init_data(fields_data); }

  /* Get data as std:vector */
  DataVector vecData()
  {
    return data.cont_vec;
  }

  /* data_t transform operator */
  operator data_t()
  {
    data_t ret = {0};
    u8 shift = 0;
    for(auto ex : length.cont_vec)
    {
      try
      {
        ret = ret | (data[ex.name] << shift);
      }
      catch(DidNotFoundDataByName<NameT>&) {}
      shift += ex.cont;
    }
    return ret;
  }

  /* BitFlow transform operator */
  operator BitFlow()
  {
    return BitFlow( (data_t) *this );
  }

  Fields& operator= (BitFlow fields_data)
  {
    init_data(fields_data);
    return *this;
  }

  Fields& operator= (FieldsData<NameT> fields_data)
  {
    data = fields_data;
    return *this;
  }

  /* Subscript operators */
  const BitFlow& operator[](NameT name) const { return data[name]; }
        BitFlow& operator[](NameT name)       { return data[name]; }

};

} /* namespace SPU */

#endif /* FIELDS_HPP */