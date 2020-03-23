/***************************************************************************
 * Project           			 : shakti devt board
 * Name of the file	     		 : rgb.c
 * Brief Description of file     :
 * Name of Author                : Soutrick Roy Chowdhury
 * Email ID                      : soutrick97@gmail.com

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

/** @fn delay_loop
 * @brief Maintains the required delay to perform an operation
 * @warning No warning
 * @param[in] unsigned long, unsigned long
 * @param[Out] No output parameter
 */
extern void delay_loop(unsigned long cntr1, unsigned long cntr2);

/** @fn main
 * @brief Getting RGB light glow one by one
 * @details By giving HIGH signal RED GREEN and BLUE light glow automatically.
 * @warning No warning
 * @param[in] No input parameter
 * @param[Out] No output parameter
 */
void main()
{
    //Assume  0 for INPUT and  1 for OUTPUT
    write_word(GPIO_DIRECTION_CNTRL_REG, 0x00000FFF);

    while (1) {
        write_word(GPIO_DATA_REG, GPIO0);
	    delay_loop(1000, 1000);
	printf("\nRED LIGHT");			//RED LIGHT.
        write_word(GPIO_DATA_REG, 0x00);
        delay_loop(1000,1000);

        write_word(GPIO_DATA_REG, GPIO1);
	    delay_loop(1000, 1000);
	printf("\nGREEN LIGHT");			//GREEN LIGHT.
        write_word(GPIO_DATA_REG, 0x00);
        delay_loop(1000,1000);
        
        write_word(GPIO_DATA_REG, GPIO2);
	    delay_loop(1000, 1000);
	printf("\nBLUE LIGHT");			//BLUE LIGHT.
        write_word(GPIO_DATA_REG, 0x00);
        delay_loop(1000,1000);
    }
}
