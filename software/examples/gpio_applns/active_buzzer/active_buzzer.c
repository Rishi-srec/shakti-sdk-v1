/***************************************************************************
 * Project           	  : shakti devt board
 * Name of the file	      : active_buzzer.c
 * Created date			  : 20.08.2019
 * Name of Author         : Soutrick Roy Chowdhury
 * Email ID               : soutrick97@gmail.com

 Copyright (C) 2019  IIT Madras. All rights reserved.

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

*****************************************************************************/
#include "platform.h"
#include "gpio.h"


//extern void delay_loop(unsigned long cntr1, unsigned long cntr2);

/** @fn main
 * @brief Correspondingly making GPIO0 as HIGH and LOW.
 * @details It will make sound when you give HIGH signal
 * @warning 
 * @param[in] 
 * @param[Out] No output parameter
 */
void main()
{
    //Assumption 1 ---> output, 0 ---> input
    write_word(GPIO_DIRECTION_CNTRL_REG, GPIO0);

    while (1) {
        write_word(GPIO_DATA_REG, 0x0000);

        delay_loop(1000, 1000);
        write_word(GPIO_DATA_REG, GPIO0);
        
        delay_loop(1000,1000);
        printf("\nWORKED SUCCESSFULLY");
    }
}
