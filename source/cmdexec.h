/*
  cmdexec.h
        - commands executor definitions

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

#ifndef CMDEXEC_H
#define CMDEXEC_H

/* Macros to switch across command formats */
#define CASE_CMDFRMT_1 case INS
#define CASE_CMDFRMT_2 case SRCH:\
                       case DEL:\
                       case NEXT:\
                       case PREV:\
                       case NSM:\
                       case NGR
#define CASE_CMDFRMT_3 case DELS:\
                       case MIN:\
                       case MAX
#define CASE_CMDFRMT_4 case AND:\
                       case OR:\
                       case NOT
#define CASE_CMDFRMT_5 case LS:\
                       case LSEQ:\
                       case GR:\
                       case GREQ

/* Macros to switch across result formats */
#define CASE_RSLTFRMT_0 case ADDS
#define CASE_RSLTFRMT_1 case DELS:\
                        case INS:\
                        case AND:\
                        case OR:\
                        case NOT:\
                        case LS:\
                        case LSEQ:\
                        case GR:\
                        case GREQ
#define CASE_RSLTFRMT_2 case SRCH:\
                        case DEL:\
                        case MIN:\
                        case MAX:\
                        case NEXT:\
                        case PREV:\
                        case NSM:\
                        case NGR

/* Type transform macros */
#define CMDFRMT_0(ptr)  ( (struct cmdfrmt_0 *) ptr )
#define CMDFRMT_1(ptr)  ( (struct cmdfrmt_1 *) ptr )
#define CMDFRMT_2(ptr)  ( (struct cmdfrmt_2 *) ptr )
#define CMDFRMT_3(ptr)  ( (struct cmdfrmt_3 *) ptr )
#define CMDFRMT_4(ptr)  ( (struct cmdfrmt_4 *) ptr )
#define CMDFRMT_5(ptr)  ( (struct cmdfrmt_5 *) ptr )
#define RSLTFRMT_0(ptr) ( (struct rsltfrmt_0 *) ptr )
#define RSLTFRMT_1(ptr) ( (struct rsltfrmt_1 *) ptr )
#define RSLTFRMT_2(ptr) ( (struct rsltfrmt_2 *) ptr )

/* Flag helpers */
#define PURE_CMD(cmd)   ( cmd&CMD_MASK )
#define SPU_CMD(cmd)    ( cmd&CMD_TO_SPU )
#define GET_Q_FLAG(cmd) ( (cmd&Q_FLAG)>>Q_FLAG_SHIFT )
#define GET_R_FLAG(cmd) ( (cmd&R_FLAG)>>R_FLAG_SHIFT )
#define GET_P_FLAG(cmd) ( (cmd&P_FLAG)>>P_FLAG_SHIFT )

/* SPU state flags helpers */
#define SPU_FLAG(state, shift) ( state & (1<<shift) )

size_t execute_cmd(const void *cmd_buf, const void **res_buf);

#endif /* CMDEXEC_H */