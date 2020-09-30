/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  flash_gpio.c
* Brief Description of file     :  artix7_35t: Reads 8 switches status
*					  and writes to flash. Reads from flash and
					  toggles the gpio pins 0 to 7.
                                          moushik: Directly write into flash 'deadbeef'
					  and read from flash and toggle the leds in
					  gpio pins. SPANSTION Flash IC NO FL256LAVFOD
* Name of Author    	                 : G Nambirajan
* Email ID                               : nambirajan2004@gmail.com

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
@file flash_gpio.c
@brief write the data in the spansionflash with spi and read the data thru spi and read data was pass in gpio pins and toggle the leds in the gpio
@detail Configures the SPI flash, Confirms the flash device id and then write the data in addresses in flash and then read and write in gpio pins
*/

#include <stdio.h>//includes the definitions of standard input/output functions
#include "gpio.h" // includes definitions of gpio pins and read, write functions
#include "spi.h"
#include "platform.h"
#define PINMUX_CONF_REG 0x40310

int * pinmux_reg  =   (const int*) PINMUX_CONF_REG;

/** @fn extern void delay_loop(unsigned long cntr1, unsigned long cntr2);
 * @brief Maintains the required delay to perform an operation 
 * @param cntr1
 * @param cntr2
 */
extern void delay_loop(unsigned long cntr1, unsigned long cntr2);

/**  @fn main
 * @brief Configures and Write the SPI values.
 * @details Configures the SPI Write FLASH values, Write the same 
 * prints the write values.
 * @param[in] No input parameter
 * @param[Out] int
 * @return Void function (Null)
 */
void main()
{
	spansion();
}

/** @fn Write & read_spantion_registers
 * @brief Writes & read the spi value to spantion
 * @details Writes & read SPI over spantion interface
 * @param[in]  unsigned int, unsigned int, unsigned char, unsigned long
 * @param[Out] int
 * @return Void function (Null)
 */
void spansion()
{
#if defined(ARTIX7_35T) 
	unsigned long readData = 0;
	//*pinmux_reg = 0x154000;
	int write_address = 0x00b00000;  // read/write from/to this address
	
	write_word(GPIO_DIRECTION_CNTRL_REG, 0x0000FFFF);
	readData =read_word(GPIO_DATA_REG);//copies the GPIO_DATA_REG Register contents//
	printf("\n Read Data is :%x\n", readData);

	int data1 = (readData >>  24);

	delay_loop(1000, 1000);
	write_word(GPIO_DIRECTION_CNTRL_REG, 0x0000FFFF);

	configure_spi(SPI1_OFFSET);	
	spi_init();
	printf("SPI init done\n");
	flash_device_id(); 

	waitfor(200);
	flash_write_enable();
	flash_erase(0x00b00000); //erases an entire sector
	flash_status_register_read();

	//flash write
	flash_write_enable();
	flash_write( write_address,data1);

	printf("Flash write done on address %x and data1 %x \n", 
	       write_address, data1);
#endif
#if defined(AARDONYX)

	int write_address = 0x00b00000;  // read/write from/to this address
	int data = 0xDEADBEEF; //32 bits of data can be written at a time

    configure_spi(SPI1_OFFSET);
	spi_init();

	printf("SPI init done\n");

	flash_device_id(); 

	waitfor(200);

	flash_write_enable();
	flash_erase(0x00b00000); //erases an entire sector
	flash_status_register_read();

	//flash write
	flash_write_enable();
	flash_write( write_address,data);
	printf("Flash write done on address %x and data %x \n", 
	       write_address, data);
#endif
	int i=4;
	configure_spi(SPI1_OFFSET);
	spi_init();
	printf("SPI init done\n");
	flash_device_id();
	waitfor(200);
	//int total_data = 10; //total data you want to read/write
	int read_address = 0x00b00000;  //read from this address
	int* bram_address = (int*) 0x80010000; //write to this address

	int read_value= flash_read(read_address);
		printf("Flash write done on address %x \n", read_address);
        printf("data1:%x \n",read_value); 
		*(bram_address+i) = read_value;

	//*pinmux_reg = 0x000;
	write_word(GPIO_DIRECTION_CNTRL_REG, 0x0000FFFF);
	printf("Flash write done on data1 %x \n",read_value);

	while (1) {
		write_word(GPIO_DATA_REG, read_value);
		delay_loop(1000,1000);
		write_word(GPIO_DATA_REG, 0x00);
		delay_loop(1000, 1000);
	}
}
