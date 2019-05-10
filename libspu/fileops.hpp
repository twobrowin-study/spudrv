/*
  fileops.hpp
        - file operation helper class declaration and implementation

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

#ifndef FILEOPS_HPP
#define FILEOPS_HPP

#include <unistd.h>
#include <fcntl.h>

namespace SPU
{

/***************************************
  File operations class definition
***************************************/

/* File operations helper */
class Fileops
{
private:
  int descriptor = 0; // Driver File Descriptor to connect SPU

public:

  /* Constructor */
  Fileops(const char* filename)
  {
    descriptor = open(filename, O_RDWR);
  }


  /* Destructor */
  ~Fileops()
  {
    if(descriptor)
    {
      close(descriptor);
    }
  }


  /* Template method witch executes given format */
  template<typename CmdFrmt, typename RsltFrmt>
  RsltFrmt execute(CmdFrmt &cmd)
  {
    void* buf    = &cmd;
    size_t count = sizeof(CmdFrmt);
    count        = write(descriptor, buf, count);
    return *(RsltFrmt *)buf;
  }
};

} /* namespace SPU */

#endif /* FILEOPS_HPP */