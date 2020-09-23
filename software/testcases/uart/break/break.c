/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  hello.c
* Brief Description of file     :  Does the printing of hello with the help of uart communication                                       protocol.
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
#include<uart.h>

/**
 * @file eparity.c
 * @project SDK project
 * @brief Checks the received character when even parity is set.
 */

/*!
Break condition:
  A break condition occurs when the receiver input is at the "space" (logic low, i.e., '0') level for longer than some duration of time, typically, for more than a character time. This is not necessarily an error, but appears to the receiver as a character of all zero-bits with a framing error. The term "break" derives from current loop signaling, which was the traditional signaling used for teletypewriters. The "spacing" condition of a current loop line is indicated by no current flowing, and a very long period of no current flowing is often caused by a break or other fault in the line.
  
  Some equipment will deliberately transmit the "space" level for longer than a character as an attention signal. When signaling rates are mismatched, no meaningful characters can be sent, but a long "break" signal can be a useful way to get the attention of a mismatched receiver to do something (such as resetting itself). Computer systems can use the long "break" level as a request to change the signaling rate, to support dial-in access at multiple signaling rates. The DMX512 protocol uses the break condition to signal the start of a new packet.


Procedure to test:
  1. Run the miniterm with no parity.
  2. Continuosly send 0(hexadecimal) from miniterm (Don't send ascii '0' (0x30). 
  3. After some time we need to get break error.
*/

void main()
{
	printf ("Hello World !\n");
	while(1)
	{
		while( 0 == (uart_instance[0]->status & ENABLE_RX_NOT_EMPTY ) );
		if(1 == (uart_instance[0]->status & BREAK_ERROR) )
			printf("\n Break error"); 
		else
			printf("\n The received char: %c", uart_instance[0]->rcv_reg);
	}

}
