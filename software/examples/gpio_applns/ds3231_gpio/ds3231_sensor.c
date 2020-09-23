/***************************************************************************
 * Project           		: shakti devt board
 * Name of the file	     	: ds3231_sensor.c
 * Brief Description of file    : Configures the RTC and reads RTC time information.
 * Name of Author    	        : Kotteeswaran, Diksha Pandey, Soma Bharti
 * Email ID                     : kottee.1@gmail.com

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
@file ds3231.c
@brief Contains the driver routines to configure and read DS3231.
@detail I2C based routines to configures and read the RTC information from DS3231.
*/
/**********************
--------------------------------------------
**RTC MODULE DS3231 CODE *******
**created by : DIKSHA PANDEY
			 : SOMA BHARTI
--------------------------------------------
***********************/
#include "gpio_i2c.h"
#include "gpio_spi.h"
#include <stdio.h>
#include <string.h> 

#define DS3231_SLAVE_ADDRESS 0XD0
#define _12_24_FORMAT 6
#define _10UNITS_REG_POS 4
#define _AMPM_BIT5 5
#define delay 800
#define EXTRACT(A) ( ( ( A & ( 0xF0 ) ) >> 4 ) * 10 ) | ( A & ( 0x0F ) )
#define EXTRACT_24_HOURS(B) ( ( ( B & (0x30)) >> 4 ) * 10 ) | ( B & ( 0x0F ) )
#define EXTRACT_12_HOURS(C) ( ( ( C & (0x10) ) >> 4 ) * 10) | ( C & (0x0F) )
#define DS3231_REG_OFFSET 0

/** @fn main
 * @brief Reads the RTC information.
 * @details Gets the current time and date info from user and updates the RTc and reads the time information from
 *          RTC chip.
 * @param[in] None.
 * @param[Out] None
 */
int main()
{
	int rstBuff[12];
	int date, month, year , day , hour, minute, seconds;
	int date1, month1, year1, hour1, minutes1, seconds1;
	char writebuf[8];
	int form, ampm, i;
	char k[8];

	writebuf[0] = 0;		//for setting the 0th register of the module 0 for address mapping while reading
	writebuf[4] = 0;		//for ignoring the day register. it is not used in this code

/*
*data input from the user
*
*returns invalid entry if user enters wrong value
*
*/

while(1)
{
	printf("\n ENTER THE PRESENT DATE");				//taking input for date 
	scanf("%d", &date);
		if(date<1 || date>31) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}


while(1)
{
	printf("\n ENTER THE PRESENT MONTH");				//taking input for month 	
	scanf("%d", &month);
		if(month<1 || month>12) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}
	
while(1)
{
	printf("\n ENTER THE PRESENT YEAR");				//taking input for year					
	scanf("%d", &year);
		if(year<0 || year>99) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}

while(1)
{
	printf("\n ENTER THE FORMAT FOR HOUR ( 1 FOR 12 HOUR FORMAT ---- AND -------- 0 FOR 24 HOUR FORMAT " );			//taking inout for 12/24 hr format
	scanf("%d" , &form );
		if(form<0 || form>1) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}


	if (0 == form)
	{
		ampm=0;
		while(1)
		{
			printf("\n ENTER THE PRESENT TIME IN HH [00 - 23]");			//taking input when 24 hr format is selected		
			scanf("%d", &hour);
			if(hour < 0 || hour > 23)
	   		{
				printf("\n INVALID ENTRY");
				printf("\n ENTER THE CORRECT VALUES");
				continue;		
			}	
			else 
				break;
		}

	}
	else 
	{
		while(1)
			{
				printf("\n ENTER 0 FOR AM AND 1 FOR PM");					//taking input for am/pm when 12 hr format is selected
				scanf("%d" , &ampm);
				if(ampm < 0 || ampm > 1)
		   		{
					printf("\n INVALID ENTRY");
					printf("\n ENTER THE CORRECT VALUES");
					continue;		
				}
				else 
					break;
			}
 	}

		
while(1)
{
	printf("\n ENTER THE PRESENT TIME IN HH");								//taking input for hr for 12 hr
	scanf("%d", &hour);
		if(hour < 1 || hour > 12)
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}


while(1)
{
	printf("\n ENTER THE PRESENT MINUTES");									//taking input for minutes 										
	scanf("%d", &minute);
		if(minute<0 || minute>59) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}


while(1)
{
	printf("\n ENTER THE PRESENT SECONDS");									//taking input for seconds
	scanf("%d", &seconds);
		if(seconds<0 || seconds>59) 
		{
			printf("\n INVALID ENTRY");
			printf("\n ENTER THE CORRECT VALUES");
			continue;
		}
		else 
			break;
}

	

/*
*
*assigning values to respective registers
*
*the following logic codes convert user defined values to register readable hexadecimal value according to the datasheet of ds3231
*
*the obtained hexadecimal values are then stored in an array writebuf which will be transferred to registers at once(see write operation below
*
*/


writebuf[1] =((seconds / 10) << _10UNITS_REG_POS) | ( seconds % 10 ); 
 printf("\n Value needs to be written: %02x", writebuf[1]);

writebuf[2] = ((minute / 10) << _10UNITS_REG_POS) | ( minute % 10);
 printf("\n Value needs to be written: %02x", writebuf[2]);

writebuf[3] = (form << _12_24_FORMAT) | ( ampm << _AMPM_BIT5 ) | (hour/10 << _10UNITS_REG_POS) | (hour % 10);
 printf("\n Value needs to be written: %02x", writebuf[3]);

writebuf[5] = ((date/10)<<_10UNITS_REG_POS) | (date%10);
 printf("\n Value needs to be written: %02x", writebuf[5]);

writebuf[6] = ((month/10)<<_10UNITS_REG_POS) | (month%10);
 printf("\n Value needs to be written: %02x", writebuf[6]);

writebuf[7] = ((year/10)<<_10UNITS_REG_POS)|(year%10);
 printf("\n Value needs to be written: %02x", writebuf[7]);




/*
*writing into the register of DS3231
*
*the array writebuf is transffered over the registers of ds3231
*
*the steps are followed as per i2c seriel bus section of the datasheet
*
*/ 


	printf("\n I2C Test Start");
	I2cInit();   
	I2cSendSlaveAddress(DS3231_SLAVE_ADDRESS,I2C_WRITE, delay);

	for(i=0; i<7; i++)
	{
		I2cWriteData( writebuf[i] ,delay) ;
		delay_loop(800, 800);
	}

	I2cStop(delay);	

/*
*reading from the register of DS3231
*
*at first the address regsiter is mapped for the read function to begin.
*
*the registers are then read sequentially according to the steps mentioned in the datasheet
*/ 


	while(1)
	{
		I2cSendSlaveAddress(DS3231_SLAVE_ADDRESS,I2C_WRITE, delay);	//writing the register address to start reading process
		I2cWriteData( DS3231_REG_OFFSET, delay) ;					//setting the 0th address register for start reading
		delay_loop(800, 800);
		I2cStop(delay);
	
		I2cSendSlaveAddress(DS3231_SLAVE_ADDRESS,I2C_READ, delay);	//sending slave address along with read instruction
		for(i=0; i<7; i++)
		{
			printf("\n\r Reading Register ");
			k[i] = readbyte(delay);
			if(i != 6)
				SendAckForRead(delay);
			else
				SendNackForRead(delay);							//Nack for the last byte while reading
	
		}
		I2cStop(delay);
		printf("\n\r Register Read");


/*
*extracting the updated data from respective registers
*
*the hexadecimal values stored in the registers are then converted to user readable format by performing reverse operations as that of the assigning values step
*
*/		


		seconds1 = EXTRACT(writebuf[1]);					//extraction of seconds
		minutes1 = EXTRACT(writebuf[2]);					//extraction of minutes

	if ( writebuf[3] & ( 0x40 ) ) 
	{
		hour1 = EXTRACT_12_HOURS(writebuf[3]) ;				//extraction of 12 hours
		if (writebuf[3] & ( 0x20 ) )
		{
			printf ( "\n HOUR IS : %d  P.M. ", hour1);
		}
		else
		{
			printf ( "\nHOUR IS : %d  A.M. ", hour1);
		}
	}
	
	else  
	{
		hour1 = EXTRACT_24_HOURS(writebuf[3]);					//extraction of 24 hours
		printf( "\n HOUR IS : %d " , hour1) ;
	}

	date1 = EXTRACT(writebuf[5]);							//extraction of date
	
	month1 = EXTRACT(writebuf[6]);							//extraction of month

	year1 = EXTRACT(writebuf[7]);							//extraction of year



	printf( "\n THE PRESENT DATE IS  %d : %d : %d " , date1, month1, year1 );				//printing the date information

	printf("\n THE PRESENT TIME IS %d : %d : %d" , hour1, minutes1, seconds1 ); 			//printing the time information

	sprintf(rstBuff, "%02d/%02d20%02d %02d-%02d-%02d", date1, month1, year1, hour1, minutes1,seconds1);	//printing date and time as string so that ASCII values are 																												transferred over the UART
	delay_loop(1000, 1000);

	}

}

