/***************************************************************************
* Project           		: shakti devt board
* Name of the file	     	: esp8266_bmp280.c
* Brief Description of file     : A sample project used to demonstrate and integration of the BMP280
* Name of Author    	        : Soutrick Roy Chowdhury
* Email ID                      : soutrick97@gmail.com

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
  * @file esp8266_bmp280.c
  * @brief sample project using bmp280 and esp8266
  * @details Sample project to demonstrate integration of bmp280 sensor and
  * transmission of data to a website.
  * BMP280 sensor is used to collect ambient temperature and pressure, ESP8266 module is  used to
  * send data to thingspeak.com
  * Prerequisite:

  * 1. ESP8266 is configured to baud rate 9600
  * 2. ESP8266 is set in station mode
  * 3. ESP8266 is configured to connect to access point.
  * 4. A channel to receive data is configured in thingspeak.com
  * 5. Change the API_key in the esp8266_bmp280.c

*/

#include "uart.h" //Includes the definitions of uart communication protocol//
#include "string.h"
#include <stdint.h>//Includes the definitions of standard input/output functions// 
#include "i2c.h"
#include "log.h"
#include "esp8266_bmp280.h"

int bmp280_calib_dig_T1;
int bmp280_calib_dig_T2;
int bmp280_calib_dig_T3;

int bmp280_calib_dig_P1;
int bmp280_calib_dig_P2;
int bmp280_calib_dig_P3;
int bmp280_calib_dig_P4;
int bmp280_calib_dig_P5;
int bmp280_calib_dig_P6;
int bmp280_calib_dig_P7;
int bmp280_calib_dig_P8;
int bmp280_calib_dig_P9;

/** @fn int write_to_esp8266(uart_struct * ptr_UART, char *data) 
 * @brief sends data to esp8266 using UART.
 * @param data Data that is send to cloud.
 * @param ptr_UART UART is connected to esp8266.
 */
int write_to_esp8266(uart_struct * ptr_UART, char *data) 
{
	while (*data != '\0') 
	{
		write_uart_character(ptr_UART, *data);
		data++;
	}
	write_uart_character(ptr_UART,'\r');
	write_uart_character(ptr_UART,'\n');
}

/** @fn int write_enter_to_esp8266(uart_struct * ptr_UART)
 * @brief sends carriage return and new line charector to esp8266.
 * @details sends carriage return and new line charector to esp8266
 *          this method is neeed to indicate end to data transmission.
 * @param ptr_UART UART is connected to esp8266.
 */
int write_enter_to_esp8266(uart_struct * ptr_UART)
{
	write_uart_character(ptr_UART,'\r');
	write_uart_character(ptr_UART,'\n');
}

/** @fn int read_from_esp8266(uart_struct * ptr_UART, char *data)
 * @brief Reads data sent by esp8266.
 * @details Reads data sent by esp8266 until one of the key word is found.
 * @param ptr_UART UART is connected to esp8266.
 * @param data responses read from esp8266, mainly used for logging
 */
int read_from_esp8266(uart_struct * ptr_UART, char *data)
{
	int ch;
	char *str = data;
	char *test = data;

	for(int i=0;i<198;i++) 
	{
		read_uart_character(ptr_UART,&ch);
		//printf("read from esp %c  \n",ch);
		*str = ch;
		str++; 
		*str = '\0';
		//printf("string is %s\n",test);
		if(strstr(test,"OK") != NULL)
		{
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"ERROR") != NULL) 
		{
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,">") != NULL) 
		{
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"RECV") != NULL) 
		{
			//printf("read from esp8266 %s\n",test);
			return;
		}
	}
	*str = '\0';
	return 1;
}

/** @fn void main()
 * @brief formats AT commands  to be sent to esp8266.
 * @details formats the data in AT commands and sends to esp8266 in  sequence
 *          Open connection to server (thingspeak.com in this example)
 *          Send data to the server 
 *          Close connection
 */
void main()
{	
	char data[200];
	int i=0;
	int baudrate = 9600;
	char send_data[200];
	char temp_string[10];
	printf("\n Sending data to cloud using exp8266!....\n");
	printf("\n Baud rate set to %d \n",baudrate);

/***************************************************

    AT ==> response OK
    AT+CWLAP ==> list nearby available WiFi networks
    AT+CIPMUX=0 ==> maximum one connection at any time
    AT+CIPSTART="TCP","api.thingspeak.com",80  ==> connect to the server
    AT+CIPSEND=75 ==>
    GET https://api.thingspeak.com/update?api_key=9T40C2TMUSU1ZX6E&field1=45 ==> send data
    Send \r\n to send data in the link
    AT+CIPCLOSE ==> to close the connection

**************************************************/

//=========================================bmp820 main===================================
	printf("-------------------------------------------START------------------------------------------\n\n");
#ifdef BMP280	
	int timeout;
	unsigned int tempReadValue = 0;
	unsigned long delay = 1000;
	int pressure = 0, temperature = 0;
	int len;
//	set_baud_rate(uart_instance[0], 115200);
		log_debug("\n\tI2C: BMP280 Temperature Sensor I2C read\n");
		i2c_init();
		//Initialises I2C Controller
			if(config_i2c(I2C, PRESCALER_COUNT,SCLK_COUNT))
			{
				log_error("\tSomething Wrong In Initialization\n");
				return -1;
			}
		else
				log_info("\tIntilization BMP280_STATUS_REGISTERHappened Fine\n");

		
		write_bmp280_register(I2C, BMP280_CONFIG_REGISTER, 0xC0, delay);
		write_bmp280_register(I2C, BMP280_CTRL_MEANS, 0x27, delay);


		if(0 == read_bmp280_register(I2C, 0xD0, &tempReadValue, delay))
		{
			if (0x58 != tempReadValue)
			{
				printf("\n Device Not detected");
				return -1;
			}
		}
		
		//printf("\nSENDING 0xE0 to OxB6 for the reset");
		write_bmp280_register(I2C, BMP280_RESET_REGISTER, 0xB6, delay);
		read_bmp280_register(I2C, BMP280_RESET_REGISTER, &tempReadValue, delay);
		

		bmp280_calib_dig_T1 = read_bmp280_values16(I2C, BMP280_REG_DIG_T1, delay);
		bmp280_calib_dig_T2 = read_bmp280_values16(I2C, BMP280_REG_DIG_T2, delay);
		bmp280_calib_dig_T3 = read_bmp280_values16(I2C, BMP280_REG_DIG_T3, delay);

		bmp280_calib_dig_P1 = read_bmp280_values16(I2C, BMP280_REG_DIG_P1, delay);
		bmp280_calib_dig_P2 = read_bmp280_values16(I2C, BMP280_REG_DIG_P2, delay);
		bmp280_calib_dig_P3 = read_bmp280_values16(I2C, BMP280_REG_DIG_P3, delay);
		bmp280_calib_dig_P4 = read_bmp280_values16(I2C, BMP280_REG_DIG_P4, delay);
		bmp280_calib_dig_P5 = read_bmp280_values16(I2C, BMP280_REG_DIG_P5, delay);
		bmp280_calib_dig_P6 = read_bmp280_values16(I2C, BMP280_REG_DIG_P6, delay);
		bmp280_calib_dig_P7 = read_bmp280_values16(I2C, BMP280_REG_DIG_P7, delay);
		bmp280_calib_dig_P8 = read_bmp280_values16(I2C, BMP280_REG_DIG_P8, delay);
		bmp280_calib_dig_P9 = read_bmp280_values16(I2C, BMP280_REG_DIG_P9, delay);


	while(1)
	{
		write_bmp280_register(I2C, BMP280_CTRL_MEANS, BMP280_NORMAL_MODE, delay );     // Set it to NORMAL MODE
		if(0 == read_bmp280_register(I2C, BMP280_STATUS_REGISTER, &tempReadValue, delay))
		{
			if(!(tempReadValue & 0x9))
				{
				//Read pressure and temperature values.
				read_bmp280_values(I2C, 0xF7, &pressure, &temperature, delay);
				//printf("\nFrom Main temp:%u\nFrom Main pressure:%d", temperature, pressure);
				}
		}
			else
			{
		//Display the error
				log_error("\nTemperature read failed.");
			}
			delay_loop(2000, 2000);
	
//========================================================================================	
#endif
#ifdef ESP8266

		set_baud_rate(uart_instance[1], 9600);
		// Connect to Wifi using esp8266
		flush_uart(uart_instance[1]);
		printf(" sending AT command to esp\n");
		write_to_esp8266(uart_instance[1], "AT");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);

#if 0
// steps to connect to a wifi hotspot

		// printf(" write AT+RST\n");
		// flush_uart(uart_instance[1]);
		// write_to_esp8266(uart_instance[1], "AT+RST");
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);

		// printf(" write AT+CWMODE\n");
		// flush_uart(uart_instance[1]);
		// write_to_esp8266(uart_instance[1], "AT+CWMODE=1");
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);

		// printf(" write AT+CWLAP\n");
		// flush_uart(uart_instance[1]);
		// write_to_esp8266(uart_instance[1], "AT+CWLAP");
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);

		// printf(" Connect to Whywifi\n");
		// flush_uart(uart_instance[1]);
		// write_to_esp8266(uart_instance[1], "AT+CWJAP=\"whywifi\",\"riselab123\"");
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);	

		// delay_loop(2000,2000);
		// printf(" write AT+CIFSR\n");
		// flush_uart(uart_instance[1]);
		// write_to_esp8266(uart_instance[1], "AT+CIFSR");
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);
#endif		

		printf(" sending AT Echo off command to esp\n");
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], "ATE0");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);

		printf(" write AT+CIPMUX\n");
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], "AT+CIPMUX=0");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);
		
		printf(" Open connection to thingspeak.com\n",data);
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);
	
		printf(" write AT+CIPSEND\n");
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], "AT+CIPSEND=93");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);

		sprintf(send_data,"GET https://api.thingspeak.com/update?api_key=%s&field1=%d.%d&field2=%d.%d",API_KEY,(temperature/100), (temperature%100),(pressure/1000),(pressure%1000));
		printf("\n send data %s\n",send_data);
		printf(" write Data\n");
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], send_data);

		// flush_uart(uart_instance[1]);
		// write_enter_to_esp8266(uart_instance[1]);
		// read_from_esp8266(uart_instance[1], data);
		// printf(" data from esp :%s\n",data);

		// disconnect from link
		printf(" write AT+CIPCLOSE\n");
		flush_uart(uart_instance[1]);
		write_to_esp8266(uart_instance[1], "AT+CIPCLOSE");
		read_from_esp8266(uart_instance[1], data);
		printf(" data from esp :%s\n",data);
		delay_loop(60000,1000);
	}
	//delay_loop(60000,1000);

#endif
}
