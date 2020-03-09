/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  switch_driver.c
 * Created date			        :  22.08.2019
 * Brief Description of file            :  Reads onboard switch values from gpio pins.
 * Name of Author    	                :  Madan Kumar S
 * Email ID                             :  kumarmadan96@gmail.com

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

 ***************************************************************************/

#if defined(ARTIX7_35T) || defined(ARTIX7_100T)

#include "platform.h"
#include "gpio.h"
#include "switch_driver.h"

extern void delay_loop(unsigned long , unsigned long );

/** @fn static void configure_switch()
 * @brief Function for configure Individual Switch pins as input.
 * @details 4 GPIO pins are mapped to 4 Switches. This function configures
 each SW as input pin.
 * @warning Nil.
 * @param[in] Pin that needs to be configured as SW.
 * @param[Out] No output parameters.
 */
void configure_switch(unsigned long pinCntrl)
{
	unsigned long read_data = 0;
	read_data = read_word(GPIO_DIRECTION_CNTRL_REG);
	write_word(GPIO_DIRECTION_CNTRL_REG, ( read_data | ~(pinCntrl) ) );
}

/** @fn static void configure_all_switchs()
 * @brief Function for configure All SWs as input.
 * @details 4 GPIO pins are mapped to 4 Switches. This function configures
 *          all SWs as input pin.
 * @warning Nil.
 * @param[in]
 * @param[Out] No output parameters.
 */
void configure_all_switchs()
{
	unsigned long read_data = 0;
	read_data = read_word(GPIO_DIRECTION_CNTRL_REG);
	write_word(GPIO_DIRECTION_CNTRL_REG, ( read_data | ~(ALL_SWITCHES) ) );
}

#endif
