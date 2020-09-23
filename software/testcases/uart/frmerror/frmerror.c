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
	Framing error
	A UART will detect a framing error when it does not see a "stop" bit at the
	expected "stop" bit time. As the "start" bit is used to identify the 
	beginning of an incoming character, its timing is a reference for the 
	remaining bits. If the data line is not in the expected state (high) when the
	"stop" bit is expected (according to the number of data and parity bits for
	which the UART is set), the UART will signal a framing error. A "break" 
	condition on the line is also signaled as a framing error.

Procedure to test:
1. Configure miniterm for 2 stop bits.
2. Send some data from miniterm to SoC UART.
3. Check the status register

*/

#include<uart.h>

void main()
{
	int value = 0;
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
		if(1 == (uart_instance[0]->status & FRAME_ERROR) )
			printf("\n Frame Parity error");
		else
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	}
}
