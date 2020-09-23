/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  baudrate.c
* Brief Description of file     :  Changes the Baud rate for  uart communication .
* Name of Author    	        :  Kotteeswaran
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
@file   baudrate.c
@brief  Tests the baudrate register in UART..
@detail Changes the baud rate and then send some data from Soc.
*/
/* 
Procedure to test
1. Configure miniterm for baudrate of 115200.
2. Check the Hello string in console.
3. Send few character from miniterm to uart.
4. The same should be received in UART and displayed in console
*/

#include<uart.h>

void main()
{
	set_baud_rate(uart_instance[0], 115200);
	printf ("Hello World !\n");
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	}
}
