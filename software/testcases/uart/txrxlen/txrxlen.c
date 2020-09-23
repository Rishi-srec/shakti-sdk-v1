/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  txrxlen.c
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
@file	 txrxlen.c
@brief	Configures and Tests the bits to be transmitted / received configuration in UART.
@detail    configures and checks by varying transmit and receive length bits .
*/
/* 
Varying transmit and receive length.
The transmit/receive length can be programmable for 1 to 32 stop bits.
Default value is 8bits.

Procedure to test
1. Configure miniterm (default stop bits).
2. Configure SoC UART for 16 bits as transmit and receive.
3. Send few characters from miniterm to uart.
4. Might get some junk character in SoC UART.
5. We wont get any data in miniterm.
*/



#include<uart.h>


void main()
{
	int value = 0;
	value = uart_instance[0]->control;
	uart_instance[0]->control = value | STOP_BITS(2);
	printf("\n Hello world");
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
		if(1 == (uart_instance[0]->status & FRAME_ERROR) )
			printf("\n Framing error");
		else
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	} 
}
