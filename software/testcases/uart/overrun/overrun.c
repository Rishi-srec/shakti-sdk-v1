/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  overrun.c
* Brief Description of file     :  Checks the occurance of overrun in UART.
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
	@file	overrun.c
	@brief	Tests the baudrate register in UART..
	@detail Changes the baud rate and then send some data from Soc.
	*/
	/* 
	Overrun error
	An "overrun error" occurs when the receiver cannot process the character 
	that just came in before the next one arrives. Various devices have 
	different amounts of buffer space to hold received characters. The CPU or 
	DMA controller must service the UART in order to remove characters from 
	the input buffer. If the CPU or DMA controller does not service the UART 
	quickly enough and the buffer becomes full, an Overrun Error will occur, 
	and incoming characters will be lost.
	
	Procedure: 
	Tries to receive continuous data in polling mode and just try to read status register without reading receive fifo buffer.
	As FIFO is not read to receive new data, overrun error will occur.
	
	Procedure to test
	1. Configure miniterm.
	2. Check the Hello string in console.
	3. Send few character from miniterm to uart.
	4. Need to get overrun error after some time.
	*/
	
#include<uart.h>
	
void main()
{
	printf ("Hello World !\n");
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
		 if(0 != (uart_instance[0]->status & OVERRUN) )
			printf("\n Overrun error");
	}
}

