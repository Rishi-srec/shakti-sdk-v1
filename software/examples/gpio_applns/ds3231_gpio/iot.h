/****************************************************************************
 * Project           	      : shakti devt board
 * Name of the file	      : iot.h
 * Brief Description of file  : header file to send and receive data over UART.
 * Name of Author             : Kotteeswaran
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

*******************************************************************************/
/**
@file iot.h
@brief Contains the driver routines to configure UART and send and receive data over UART..
@detail UART based routines to send and receive data to and from server..
*/
/*****************************
***This file contains the UART FUNCTIONS TO SEND AND RECIEVE DATA FOR GATEWAY INTERFACING.
*Note: The code is not tested.
****************************/

#define UART0_BASE_ADDRESS 0x11200
#define UART_LINE_STATUS_REGISTER 5
#define UART1 1
#define UART1_BASE_ADDRESS 0x11300


/** @fn int Delay(uint16_t count1, uint16_t count2)
 * @brief Delays the controller.
 * @details Adds count1 * count2 number of cycles delay in the controller. 
 * @param count1
 * @param count2
 * @return None
 */
void Delay(uint16_t Cnt1, uint16_t Cnt2)
{
	uint16_t i = 0, j = 0;
	for( i = 0; i < Cnt1; i++)
		for( j = 0; j < Cnt2; j++);
}

/** @fn int ReadFromUart(unsigned char offSet, unsigned char uartSel)
 * @brief Reads data from UART..
 * @details Selects the UART and receives data from UART Selected.. 
 * @param offset (Address for UART receive buffer)
 * @param UART that is receiving data.
 * @return read value.
 */
unsigned char ReadFromUart(unsigned char offSet, unsigned char uartSel)
{
	int * readAddress = NULL;
	if(1 == uartSel )
		readAddress = (UART1_BASE_ADDRESS + offSet);
	else
		readAddress = (UART0_BASE_ADDRESS + offSet);
	return (*readAddress) ;
}


/** @fn int WriteIntoUart(unsigned char offSet, unsigned char uartSel, unsigned char writeData)
 * @brief Writes data into UART..
 * @details Selects the UART and sends data to the UART Selected.. 
 * @param offset (Address for UART transmit buffer)
 * @param UART that is sending data.
 * @return void.
 */
void WriteIntoUart(unsigned char offSet, unsigned char uartSel, unsigned char writeData)
{
	int * writeAddress = NULL;
	if(1 == uartSel )
		writeAddress = (UART1_BASE_ADDRESS + offSet);
	else
		writeAddress = (UART0_BASE_ADDRESS + offSet);

		*writeAddress  = writeData;

}


/** @fn int UartTxReady()
 * @brief Checks the readiness of UART.
 * @details Checks whether the transmit buffer of UART is empty to put the 
 *          next data for transmit.
 * @param None
 * @return read data (status register)
 */
unsigned int UartTxReady()
{
	unsigned int readData = 0;
	readData = ReadFromUart(UART_LINE_STATUS_REGISTER, 1);
	return readData;
}




/** @fn int UartTransmitCharacter(char c)
 * @brief Writes data using UART.
 * @details Selects the UART and sends data over the selected UART. 
 * @param data that needs to be transmitted.
 * @return void.
 */
 void UartTransmitCharacter(char c)
{
	unsigned int status, tx_empty;


	while(1)
	{
		status = UartTxReady();
		if( 1 == ( (status >> 5) & 0x1) )
			break;
	}
	// tx must be empty now so send
	WriteIntoUart(UART_DATA_REGISTER, UART1, (unsigned int) c);
}



/** @fn int UartTransmitString(char *s)
 * @brief Writes data string using UART.
 * @details Selects the UART and sends data over the selected UART. 
 * @param data that needs to be transmitted.
 * @return void.
 */
void UartTransmitString(char *s)
{
	for(; *s != NULL; s++)
		UartTransmitCharacter(*s);
}


/** @fn int UartCheckReceiveReady(char *s)
 * @brief reads the line status register.
 * @details Reads the line status register and checks whether the Receive buffer
 *          if empty or not. 
 * @param None.
 * @return 1 if not empty.
 */
int UartCheckReceiveReady(void)
{
	unsigned int readData = 0;
	readData = ReadFromUart(UART_LINE_STATUS_REGISTER, UART1);
	return (readData & 0x01);
}


/** @fn int UartReceiveCharacter(char *s)
 * @brief receives a byte of over UART.
 * @details Selects the UART and receives data over the selected UART. 
 * @param None.
 * @return received data.
 */
char UartReceiveCharacter(void)
{
	while(!UartCheckReceiveReady()) ;
	return (unsigned char) ReadFromUart(UART_DATA_REGISTER, UART1);
}



/** @fn int UartReceiveString(void)
 * @brief receives  a string over UART.
 * @details Selects the UART and receives data over the selected UART. 
 * @param None.
 * @return received data.
 */
char * UartReceiveString(void)
{	
	int i=0;
    char *buff = NULL;
	while( UartReceiveCharacter() != NULL )
		{
		   buff[i] = UartReceiveCharacter;
           i++;
		}
	return *buff;
		
}

