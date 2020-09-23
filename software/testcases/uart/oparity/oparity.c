/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  eparity.c
* Brief Description of file     :  Checks for even parity with uart.
* Name of Author    	        :  Kotteeswaran E
* Email ID                      :  kottee.1@gmail.com

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
/**
 * @file eparity.c
 * @project SDK project
 * @brief Checks the received character when even parity is set.
 */

/**! Procedure to test
	1. Configure SoC UART for odd partity.
	2. Configure miniterm for odd parity.
	3. Send data from miniterm to SoC.
	4. Read data in the code. We will be getting right data. Also check the parity 
	   error bit in status register. We won't get any error.
	5. Configure miniterm for even parity.
	6. Read data in the code. We won't be getting right data. Also check the parity 
	   error bit in status register. We will get error.
	5. Configure miniterm for no parity.
	7. Read data in the code. We won't be getting right data. Also check the parity 
	   error bit in status register. We will get error.

	SoC transmits data.
	1. Configure UART for odd partity.
	2. Configure miniterm for odd parity.
	3. Send data from SoC to miniterm.
	4. Read data in the miniterm. We will be getting right data. 
	5. Configure miniterm for even parity.
	6. Read data in the SoC. We won't be getting right data. The data will be junk.
	5. Configure miniterm for no parity.
	7. Read data in the code. We won't be getting right data. The data will be junk.
*/

#include<uart.h>

/** @fn main
 * @brief prints hello world
 */
void main()
{
	int value = 0;
	value = uart_instance[0]->control;
	uart_instance[0]->control = value | PARITY(1);
	printf("\n Hello world" );
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
		if(1 == (uart_instance[0]->status & PARITY_ERROR) )
			printf("\n Odd Parity error");
		else
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	}
}
