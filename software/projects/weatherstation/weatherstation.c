/***************************************************************************
* Project           			:  shakti devt board
* Name of the file	     		:  weatherstation.c
* Brief Description of file             :  A sample project to demonstrate 
					   integration of temperature sensor 
                                           and transmission of data to a website
* Name of Author    	                :  Anand Kumar S
* Email ID                              :  007334@imail.iitm.ac.in

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
  * @file weatherstation.c
  * @brief sample project using temperature sensor and esp8266
  * @details Sample project to demonstrate integration of temperature sensor and
  * transmission of data to a website.
  * LM75 sensor is used to collect ambient temperature, ESP8266 module is  used to
  * send data to thingspeak.com
  * Prerequisite:

  * 1. ESP8266 is configured to baud rate 9600
  * 2. ESP8266 is set in station mode
  * 3. ESP8266 is configured to connect to access point.
  * 4. A channel to receive data is configured in thingspeak.com
  * 5. Change the API_key in the weatherstation.c

*/

#include "uart.h"//Includes the definitions of uart communication protocol//
#include "string.h"
#define LM75_SLAVE_ADDRESS 0x90//Defines the Starting address of slave
#define API_KEY "9T40C2TMUSU1ZX6E"

/**
  * @fn int write_to_esp8266(char *data,uart_num sel,int baudrate)
  * @brief sends data to esp8266 using UART
  * @param data data that has to be sent to could
  * @param sel Uart that is connected to esp8266
  * @param baudrate Baudrate that is used to communicate with esp8266
*/

int write_to_esp8266(char *data,uart_num sel,int baudrate) {
	while (*data != '\0') {
		write_uart_character(ptr_UART, *data);
		data++;
	}
	write_uart_character(ptr_UART,'\r');
	write_uart_character(ptr_UART,'\n');

}

/**
  * @fn int write_to_esp8266(char *data,uart_num sel,int baudrate)
  * @brief sends carriage return and new line charector to esp8266
  * @detail sends carriage return and new line charector to esp8266
  *         this method is neeed to indicate end to data transmission
  * @param data data that has to be sent to could
  * @param sel Uart that is connected to esp8266
  * @param baudrate Baudrate that is used to communicate with esp8266
*/

int write_enter_to_esp8266(uart_num sel,int baudrate){
	write_uart('\r',sel,baudrate);
	write_uart('\n',sel,baudrate);
}

/**
  * @fn int read_from_esp8266(char *data,uart_num sel,int baudrate)
  * @brief Reads data sent by esp8266
  * @details Reads data sent by esp8266 until one of the key word is found
  * @param data responses read from esp8266, mainly used for logging
  * @param sel Uart that is connected to esp8266
  * @param baudrate Baudrate that is used to communicate with esp8266
*/
int read_from_esp8266(char *data,uart_num sel,int baudrate) {
	int ch;
	char *str = data;
	char *test = data;
	for(int i=0;i<198;i++) {
		read_uart_character(ptr_UART,&ch);
		//printf("read from esp %c  \n",ch);
		*str = ch;
		str++; 
		*str = '\0';
		//printf("string is %s\n",test);
		if(strstr(test,"OK") != NULL)  {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"ERROR") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,">") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
		if(strstr(test,"RECV") != NULL) {
			//printf("read from esp8266 %s\n",test);
			return;
		}
	}
	*str = '\0';
	return 1;
}

/**
  * @fn void transmit_data(int temperature, int baudrate)
  * @brief formats AT commands  to be sent to esp8266
  * @details formats the data in AT commands and sends to esp8266 in  sequence
  *      Open connection to server (thingspeak.com in this example)
  *      Send data to the server 
  *      Close connection
  * @param temperature temperate that has to be sent to database in cloud
  * @param baudrate baudrate used to communicate with esp8266
*/
void transmit_data(int temperature, int baudrate) {
/***************************************************
    AT – response OK
    AT+CWLAP – list nearby available WiFi networks
    AT+CIPMUX=0
    AT+CIPSTART="TCP","api.thingspeak.com",80
    AT+CIPSEND=75
    GET https://api.thingspeak.com/update?api_key=9T40C2TMUSU1ZX6E&field1=45

**************************************************/


	char data[200];
	char sendData[76];
	sprintf(sendData,"GET https://api.thingspeak.com/update?api_key=%s&field1=%d",API_KEY,temperature);
	//printf("\n send data %s\n",sendData);
	// Connect to Wifi using esp8266
		set_baud_rate(UART1, 9600);

		// Connect to Wifi using esp8266
		flush_uart(UART1);
		printf(" sending AT command to esp\n");
		write_to_esp8266(UART1, "AT");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
	
		flush_uart(UART1);
		printf(" sending AT Echo off command to esp\n");
		write_to_esp8266(UART1, "ATE0");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
	
	
		printf(" write AT+CIPMUX\n");
		flush_uart(UART1);
		write_to_esp8266(UART1, "AT+CIPMUX=0");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
s	
		delay_loop(2000,2000);
		printf(" Open connection to thingspeak.com\n",data);
		flush_uart(UART1);
		write_to_esp8266(UART1, "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
	
		//delay_loop(2000,2000);
		printf(" write AT+CIPSEND\n");
		flush_uart(UART1);
		write_to_esp8266(UART1, "AT+CIPSEND=75");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
	
		printf(" write Data\n");
		flush_uart(UART1);
		write_to_esp8266(UART1, sendData);
	
		//flush_uart(UART1);
		write_enter_to_esp8266(UART1s);
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);
	
		// disconnect from link
		printf(" write AT+CIPCLOSE\n");
		flush_uart(UART1);
		write_to_esp8266(UART1, "AT+CIPCLOSE");
		read_from_esp8266(UART1, data);
		printf(" data from esp :%s\n",data);

}

/**
  * @fn void main()
  * @brief Entry point for the program
*/
void main()
{	
	int i=0;
	int baudrate = 9600;
	int temperature = 24;
	printf("\n Sending temperature data to cloud server!....\n");
	printf("\n Baud rate set to %d \n",baudrate);
	printf("\n Waiting to ESP8266 to initialize \n");
	delay(3);
	while (1) {
		temperature = temperature_value();
		if (temperature != 999)
			transmit_data(temperature, baudrate); 
		delay(60);
	}
}
