/***************************************************************************
* Project                               :  shakti devt board
* Name of the file                      :  lm75.c
* Brief Description of file             :  A sample project to demonstrate 
                                           integration of temperature sensor 
                                           and transmission of data to a website
* Name of Author                        :  Kotteeswaran
* Email ID                              :  kottee.1@gmail.com

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
@file   lm75.c
@brief  Reads room temperature.
@detail Reads the temperature from LM75.
*/

#include <stdint.h>//Includes the definitions of standard input/output functions//
#include "i2c.h"
#define DELAY_VALUE 900 
#define DEV_IDENTITY_DATA_REG 0X75
#define DEV_IDENTITY  0X68
#define LM75_SLAVE_ADDRESS 0x90//Defines the Starting address of slave//
#define LM75_I2C 1


/** @fn int temperature_value(void)
 * @brief reads temperature value from lm75 and prints the same.
 * @details Configures the LM75, Reads the encoded temperature value, 
 * calculates the actual temperature value and prints the same.
 * @return 999 on failure; else infinite loop to read temperature.
 */
int temperature_value(void)
{
	int timeout;
	unsigned int temperature_read = 0;
	unsigned char temp_data = 0;

	//printf("\tI2C: Starting Transaction\n");


	char writebuf1[2] = {0,0};
	char writebuf[18], writeData = 0x30;
	char writeBuf[18] = {'\0'};
	writebuf[0] = 0;
	writebuf[1] = 0;

	char readbuf[32];
	int i = 0, j = 0,  k = 0, status=0;
	unsigned char temp = 0;

	if(shakti_init_i2c(0x1F,0x91))
	{
			printf("\tSomething Wrong In Initialization\n");
			return 999;
	}
else
	printf("\tIntilization Happened Fine\n");

//Selects the control register
	while(WaitForI2cBusToFree())
	{
	 printf("\tError in Waiting for BB\n");
	 return 999;
	}
	//				I2cSendSlaveAddress(DS3231_SLAVE_ADDRESS,I2C_WRITE, delay);	//writing the register address to start reading process
	set_i2c_shakti(i2c_data, LM75_SLAVE_ADDRESS );
	temp = get_i2c_shakti(i2c_data); //Will this work?
	if((LM75_SLAVE_ADDRESS) != (int)temp)
		printf("\tSomewhere something is wrong with the controller - it is giving out some random address -- Diagnose\n");

		//printf("\nSlave address for write is written successfully.");
	i2c_start(); //Asking the controller to send a start signal to initiate the transaction
	while(WaitForTxRxOperationToComplete(&status))
	{
		printf("\twaiting for pin\n");
		waitfor(DELAY_VALUE);
	}

	set_i2c_shakti(i2c_data, 0x00);
	while(WaitForTxRxOperationToComplete(&status))
	{
		printf("\twaiting for pin\n");
		waitfor(DELAY_VALUE);
	}

	delay(1);
	i2c_stop();
	delay(1);


	while(1)
	{
		//Implementing Read and Write through Polling for now
		while(WaitForI2cBusToFree())
		{
		 printf("\tError in Waiting for BB\n");
		 return 999;
		}
	//printf("\tSlave Address for read: %d\n", i);
		set_i2c_shakti(i2c_data, LM75_SLAVE_ADDRESS | 1);
		temp = get_i2c_shakti(i2c_data); //Will this work?
		if((LM75_SLAVE_ADDRESS | 1) != (int)temp)
			printf("\tSomewhere something is wrong with the controller - it is giving out some random address -- Diagnose\n");

	//printf("Slave address for read is written successfully./n");
		i2c_start(); //Asking the controller to send a start signal to initiate the transaction
		while(WaitForTxRxOperationToComplete(&status))
		{
			printf("\twaiting for pin\n");
			waitfor(DELAY_VALUE);
		}

		  get_i2c_shakti(i2c_data);
//Reads the MSB (D9 - D1) of temperature
		while(WaitForTxRxOperationToComplete(&status))
		{
			printf("\twaiting for pin\n");
			waitfor(DELAY_VALUE);
		}
		temp_data = get_i2c_shakti(i2c_data);
		//printf("\n MSB Byte Read Value[0]: %x", temp_data);
		temperature_read = temp_data << 1;
		
//Reads the LSB byte (D0) of Temperature
		while(WaitForTxRxOperationToComplete(&status))
		{
			printf("\twaiting for pin\n");
			waitfor(DELAY_VALUE);
		}
		delay(1);
		i2c_nack();
		temp_data = get_i2c_shakti(i2c_data);
		//printf("\n LSB Byte Read Value[1]: %x", temp_data);
	
		while(WaitForTxRxOperationToComplete(&status))
		{
			printf("\twaiting for pin\n");
			waitfor(DELAY_VALUE);
		}
		temperature_read = temperature_read | ((temp_data >> 7) & 0x01);

		if(temperature_read & 0x100)
		{
			temperature_read = ( (~temperature_read) + 1 ) >> 1;
			//printf("\n\t Current Temperature:=-%u°C",temperature_read);
		}
		else
		 {
			temperature_read = temperature_read >> 1;
			//printf("\n\t Current Temperature:=+%u°C",temperature_read);
		 }

		delay(1);
		i2c_stop();
		return temperature_read;
	}
return 999;
}
