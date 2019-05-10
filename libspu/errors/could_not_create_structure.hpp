/*
  errors/could_not_create_structure.hpp
        - error when there is no space in SPU's structures

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

#ifndef COULD_NOT_CREATE_STRUCTURE_HPP
#define COULD_NOT_CREATE_STRUCTURE_HPP

#include <stdexcept>
#include <string>

namespace SPU
{

/* Exception throws when there is no chance to crate another new structure */
struct CouldNotCreateStructure : public std::exception
{
  const char * what () const throw ()
    {
      return "Could not create new structure: no enough space available";
    }
};

} /* namespace SPU */

#endif /* COULD_NOT_CREATE_STRUCTURE_HPP */