/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  stopbits.c
* Brief Description of file      :  Checks the stop bits configuration .
* Name of Author    	         :  Kotteeswaran
* Email ID                                    :  kottee.1@gmail.com

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
@file	 stopbits.c
@brief	Tests the stop bits configuration in UART.
@detail    configures and checks for stop bits .
*/
/* 
Overrun error
The STOP bits in UART is programmable for 1, 1.5, 2 stop bits.

Procedure to test
1. Configure miniterm (default stop bits).
2. Configure SoC UART for 2 stop bits.
3. Send few characters from miniterm to uart.
4. Need to get overrun error after some time.
5. We wont get any data in miniterm.
6. There will be framing error in the SoC UART.
*/



#include<uart.h>


void main()
{
	int value = 0;
	value = uart_instance[0]->control;
	uart_instance[0]->control = value | UART_TX_RX_LEN(15);
	printf("\n Hello world");
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	} 
}
