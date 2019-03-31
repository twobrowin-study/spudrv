/*
  gsidresolver.h
        - container of structures by GSIDs
        - copy unused structures into PC's RAM

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

#ifndef GSIDRESOLVER_H
#define GSIDRESOLVER_H

int create_gsid(u32 gsid[GSID_WEIGHT]);
int resolve_gsid(const u32 gsid[GSID_WEIGHT], u8 cmd);

/* Macro of two GSID's equality */
/* Only GSID_WEIGHT = 4 supports */
#if GSID_WEIGHT == 4
  #define GSID_EQUAL(a, b) \
    (a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3])
#endif


#endif /* GSIDRESOLVER_H */