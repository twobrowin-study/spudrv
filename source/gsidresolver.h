/*
  gsidresolver.h
        - container of structures by GSIDs
        - copy unused structures into PC's RAM

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

#ifndef GSIDRESOLVER_H
#define GSIDRESOLVER_H

int create_gsid(gsid_t *gsid);
int resolve_gsid(gsid_t gsid, u8 cmd);

/* Macro of two GSID's equality */
/* Only GSID_WEIGHT = 4 supports */
#if GSID_WEIGHT == 4
  #define GSID_EQUAL(a, b) \
    ((a).cont[0] == (b).cont[0] && (a).cont[1] == (b).cont[1] && (a).cont[2] == (b).cont[2] && (a).cont[3] == (b).cont[3])
#endif

/* Acting SPU structure number macro */
#define SPU_STR(i) (i+1)


#endif /* GSIDRESOLVER_H */