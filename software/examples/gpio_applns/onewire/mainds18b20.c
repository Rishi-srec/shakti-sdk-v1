/***************************************************************************
 * Project           	      : shakti devt board
 * Name of the file	      : mainds18b20c.c
 * Brief Description of file  : Reads temperature over one wire interface.
 * Name of Author             : Sanjay Kumar, Kotteeswaran
 * Email ID                   : kottee.1@gmail.com

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
/**
@file mainds18b20s.c
@brief configures and reads temperature using one wire interface.
@detail Configures DS18B20S and then reads the temperature using one wire interface.
*/

#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "platform.h"
#define skip_rom 0xCC
#define	convert_temp 0x44
#define	write_scratchpad 0x4E
#define	resolution_12bit 0x7F
#define read_scratchpad 0xBE

/** @fn void main()
 * @brief Reads temperature
 * @details Configures the DS18B20S and reads temperature using one wire interface.
 * @Param None
 * @Return None.  
 */
void main()
{
   unsigned short TempL, TempH;
   unsigned int t, t2;
   int i=0;

   printf("\nDS18B20-1");
  /* This is for presence detection of temp-sensing probe*/	

   if( StartSignal() == 0)                          // initilization of sequence 
   
   printf("\ntemp sensor probe is connected");
   else 
   
   printf("\nprobe is not connected");
   printf("\nDS18B20-3");

   write_byte(write_scratchpad);                   // write into the 4EH register
   printf("\nDS18B20-4");
   write_byte(0);                                  // writing into the byte 2 or TH register
   write_byte(0);                                  // writing into the byte 3 or TL register
   printf("\nDS18B20-5");
   write_byte(resolution_12bit);                   // 12bit resolution
   printf("\nDS18B20-6");

 
   // initiliazation
   if( StartSignal() == 0)
   printf("\ntemp sensor probe is connected");
   else 
   printf("\nprobe is not connected");
   printf("\nDS18B20-7");

   write_byte(skip_rom);                          //writing into the skip rom cch ,
   write_byte(convert_temp);                      // start temperature conversion register(44h)
   printf("\nDS18B20-8");

   while (read_byte()==0xff);                    // if all value 11111111 wait for some time
   wait_for(500000);                             // waitfor 500 ms

   // initiliazation
   if( StartSignal() == 0)
   printf("\ntemp sensor probe is connected");
   else 
   printf("\nprobe is not connected");
   printf("\nDS18B20-8");

   TempL = read_byte();
   TempH = read_byte();
   printf("\nDS18B20-9");
/*This is for Negative temperature*/


   if((TempH & 0x80)!=0){                              // If condition will execute as TempH = 1111 1111 & 1000 0000 = 1000 0000.
            printf("\nfor negative temperature\n");
            printf("\nDS18B20-10");
			t=TempH;                       // Store tempH value in t = 1111 1111  .
	        t<<=8;                                 //after bitwise left shift 8 times value in t will be 1111 1111 0000 0000.
	        t=t|TempL;                             // t = 1111 1111 0000 0000 | 0101 1110 [ result t = 1111 1111 0101 1110]
            printf("\nDS18B20-11");
	        t = ~t;                                // taking the negation for 2s complement of 
            t += 1;
            t = t*0.0625;                              // 1 decimal corresponding temperature value is 0.0625 
            printf("\n %u",(unsigned int)t);
}
/*This is for positive Temperature*/
	else {
            printf("\nfor positive temperature \n");
			i=((unsigned int)TempH << 8 ) + (unsigned int)TempL; //put both value in one variable
			i = i * 0.0625;                //calculations used from the table provided in the data sheet of ds18b20
            printf("\n %d",i);

}
}
