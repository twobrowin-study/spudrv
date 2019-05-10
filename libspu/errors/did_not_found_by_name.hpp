/*
  errors/did_not_found_by_name.hpp
        - error from found_by_name function in fields data container classes

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

#ifndef DID_NOT_FOUND_BY_NAME_HPP
#define DID_NOT_FOUND_BY_NAME_HPP

#include <stdexcept>
#include <string>

namespace SPU
{

/* Exception throws when data was not found in fields data or length containers */
template <typename NameT>
struct DidNotFoundDataByName : public std::exception
{
  std::string str_what;

  DidNotFoundDataByName(std::string class_meaning, NameT exception_field_name) : std::exception()
  {
    str_what = "Did not found '" + class_meaning + "' with name " + str_what_field_name(exception_field_name);
  }

  std::string str_what_field_name(NameT exception_field_name);

  const char * what () const throw ()
    {
      return str_what.c_str();
    }
};

/* DidNotFoundDataByName with std::string names */
template <>
std::string DidNotFoundDataByName<std::string>::str_what_field_name(std::string exception_field_name)
{
  return "'" + exception_field_name + "'";
}

/* DidNotFoundDataByName with char names */
template <>
std::string DidNotFoundDataByName<char>::str_what_field_name(char exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with short names */
template <>
std::string DidNotFoundDataByName<short>::str_what_field_name(short exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with int names */
template <>
std::string DidNotFoundDataByName<int>::str_what_field_name(int exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with long names */
template <>
std::string DidNotFoundDataByName<long>::str_what_field_name(long exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with long long names */
template <>
std::string DidNotFoundDataByName<long long>::str_what_field_name(long long exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with unsigned short names */
template <>
std::string DidNotFoundDataByName<unsigned char>::str_what_field_name(unsigned char exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with unsigned char names */
template <>
std::string DidNotFoundDataByName<unsigned short>::str_what_field_name(unsigned short exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with unsigned int names */
template <>
std::string DidNotFoundDataByName<unsigned int>::str_what_field_name(unsigned int exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with unsigned long names */
template <>
std::string DidNotFoundDataByName<unsigned long>::str_what_field_name(unsigned long exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with unsigned long long names */
template <>
std::string DidNotFoundDataByName<unsigned long long>::str_what_field_name(unsigned long long exception_field_name)
{
  return "'" + std::to_string(exception_field_name) + "'";
}

/* DidNotFoundDataByName with Unknown names */
template <typename NameT>
std::string DidNotFoundDataByName<NameT>::str_what_field_name(NameT exception_field_name)
{
  return "'Unknown'";
}

} /* namespace SPU */

#endif /* DID_NOT_FOUND_BY_NAME_HPP */