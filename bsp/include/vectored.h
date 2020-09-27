/***************************************************************************
 * Project                          : shakti-devt board
 * Name of the file                 : vectored.h
 * Brief Description of file        : Header file for vector based interrupt
 * Name of Author                   : Prajwal Prakash
 * Email ID                         : prajwal.prakash2000@gmail.com

 Copyright (C) 2020  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*******************************************************************************/
/**
 * @file vectored.h
 * @project shakti devt board
 * @brief Header file for vector based interrupts
 */

#ifndef VECTORED_H
#define VECTORED_H


void enable_vectored();

unsigned int m_s_wrapper();

unsigned int m_t_wrapper();

unsigned int m_e_wrapper();

unsigned int s_s_wrapper();

unsigned int s_t_wrapper();

unsigned int s_e_wrapper();

unsigned int u_t_wrapper();

unsigned int u_e_wrapper();

unsigned int res_0_wrapper();

unsigned int res_1_wrapper();

unsigned int res_2_wrapper();

unsigned int res_3_wrapper();

unsigned int res_4_wrapper();

unsigned int res_5_wrapper();

unsigned int res_6_wrapper();

#endif
