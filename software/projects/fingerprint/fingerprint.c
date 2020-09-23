/**
 * @file fingerprint.c
 *
 * @author Matthew Matz
 *
 * @version 0.50
 *
 * @copyright
 * Copyright (C) Parallax, Inc. 2017. All Rights MIT Licensed.
 *
 * @brief Simplifies reading the WaveShare fingerprint scanner module.
 */

#include "stdio.h"
#include "fingerprint.h"
#include "uart.h"
#define FDSERIAL_BUFF_MASK 0x3f

fpScanner *fpScan;

//printf("hi i am in the program");
int userID, result;


int main()
{

  
  fpScan = fingerprint_open(UART1, 57600);
  
  printf("Deleting all users\n");

 // result = fingerprint_deleteUser(UART1,57600);  // deletes all users

  printf("Result = %d\n\n", result); // print result: 0 = ACK_SUCCESS

//  delay_loop(1000,1000);

  printf("Add fingerprint (3 impressions)\n");

  result = fingerprint_add(UART1, 15, 3, 0);  // adds fingerprint, sets userID to 15 and user level to 3.  Makes 3 scans.

  printf("Add FP result = %d\n\n", result); // print result: 0 = ACK_SUCCESS

  while(1)
  {
    delay_loop(1000,1000);

    printf("Scan your fingerprint\n");

    result = fingerprint_scan(UART1, 0, &userID);

    printf("UserID = %d, result = %d\n", userID, result);
  }

}

/*opening*/

fpScanner *fingerprint_open(uart_num uart_number, int baudrate)
{
  int mode = 0;
/*int txpin = pin_tx;
  int rxpin = pin_rx;*/

  extern int binary_pst_dat_start[];

  fpScanner_t *fpsptr;

  /* can't use array instead of malloc because it would go out of scope. */
 /* char* bufptr = (char*) malloc(2*(FDSERIAL_BUFF_MASK+1));
  fpScanner* term = (fpScanner*) malloc(sizeof(fpScanner));
  memset(term, 0, sizeof(fpScanner));

  fpsptr = (void*) malloc(sizeof(fpScanner_t));
  term->devst = fpsptr;
  memset((char*)fpsptr, 0, sizeof(fpScanner_t));
*/
/*  if(rxpin == 31 && txpin == 30) {
    simpleterm_close();
  }*/

  /* required for terminal to work */
 // term->txChar  = fdserial_txChar;
 // term->rxChar  = fdserial_rxChar;

 // fpsptr->rx_pin = rxpin;  /* recieve pin */
 // fpsptr->tx_pin = txpin;  /* transmit pin */
 // fpsptr->mode   = mode;   /* interface mode */
  //fpsptr->en     = enablePin;  /* interface mode */

  /* baud from clkfreq (cpu clock typically 80000000 for 5M*pll16x) */
 // fpsptr->ticks   = CLKFREQ/baudrate;
 // fpsptr->buffptr = bufptr; /* receive and transmit buffer */

 // term->cogid[0] = setStopCOGID(cognew((void*)binary_pst_dat_start, (void*)fpsptr));
	uart_init(uart_number, baudrate);
  
delay_loop(1000,1000);
 // return term;
}



void fingerprint_close(uart_num uart_number)
{
#if 0 
int id = device->cogid[0];
  fpScanner_t *fpsptr = (fpScanner_t*) device->devst;

  while(fdserial_rxCheck(device) >= 0);   // clear out queue by receiving all available 
  fdserial_txFlush(device);

  if(id > 0) cogstop(getStopCOGID(id));
  
  free((void*)fpsptr->buffptr);
  free((void*)fpsptr);
  free(device);
  device = 0;
#endif
}



void fingerprint_sendCommand(uart_num uart_number, char __fpCmd, char __fpParam1, char __fpParam2, char __fpParam3)
{
  char __fpChk = __fpCmd;
  __fpChk ^= __fpParam1;
  __fpChk ^= __fpParam2;
  __fpChk ^= __fpParam3;
  __fpChk ^= 0;
  
  write_uart(0xF5, uart_number);
  write_uart( __fpCmd,uart_number);
  write_uart(__fpParam1,uart_number);
  write_uart(__fpParam2,uart_number);
  write_uart(__fpParam3, uart_number);
  write_uart(0, uart_number);
  write_uart(__fpChk, uart_number);
  write_uart(0xF5, uart_number);
  
}  



void fingerprint_readResponse(uart_num uart_number, char *__fpResponse)
{
  char __read_fp[8];
  char __fpChk = 0;
  for(char idx = 0; idx < 8; idx++)
  {
    __read_fp[idx] = read_uart( uart_number);


    if(idx > 0 && idx < 6) __fpChk ^= __read_fp[idx];
    if(idx > 1 && idx < 5) __fpResponse[idx-2] = __read_fp[idx];
  }
  __fpResponse[4] = 0;
  if(__read_fp[0] == 0xF5 && __read_fp[7] == 0xF5 && __read_fp[6] == __fpChk) __fpResponse[5] = 0xF5;
}



int fingerprint_allowOverwrite(uart_num uart_number, char b) 
{
  char __fpBuf[8];
  char __allowOverwrite = 0;
  if(b == 0) __allowOverwrite = 1;
  fingerprint_sendCommand(uart_number, CMD_SET_MODE, 0, __allowOverwrite, 0);
  fingerprint_readResponse(uart_number, __fpBuf);
  return (int) __fpBuf[2];
}  



int fingerprint_add(uart_num uart_number, int userId, char userLevel, int scanNumber) 
{
  userLevel = 0x3 & userLevel;
  char __u2 = userId & 0xFF;
  char __u1 = (userId >> 8) & 0xFF;
  char __fpBuf[8];
  
  if(userId < 1) return ACK_NOUSER;
  else {
    if(scanNumber < 1 || scanNumber > 3) {
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_1, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
      if(__fpBuf[2] != ACK_SUCCESS) return (int) __fpBuf[2];
      delay_loop(1000,1000);
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_2, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
      if(__fpBuf[2] != ACK_SUCCESS) return (int) __fpBuf[2];
      delay_loop(1000,1000);
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_3, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
    } else if(scanNumber == 1) {
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_1, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
    } else if(scanNumber == 2) {
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_2, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
    } else if(scanNumber == 3) {
      fingerprint_sendCommand(uart_number, CMD_ADD_FINGERPRINT_3, __u1, __u2, userLevel);
      fingerprint_readResponse(uart_number, __fpBuf);
    }
        
    return (int) __fpBuf[4];  
  }    
}  



int fingerprint_deleteUser(uart_num uart_number, int userId) 
{
  char __u2 = userId & 0xFF;
  char __u1 = (userId >> 8) & 0xFF;
  char __fpBuf[8];
  
  if(userId < 1) {
    fingerprint_sendCommand(uart_number, CMD_DELETE_ALL_USERS, 0, 0, 0);    
    fingerprint_readResponse(uart_number, __fpBuf);
    //high(26);
  } else {
    fingerprint_sendCommand(uart_number, CMD_DELETE_USER, __u1, __u2, 0);    
    fingerprint_readResponse(uart_number, __fpBuf);
  }
  
  return (int) __fpBuf[4];  
}



int fingerprint_countUsers(uart_num uart_number) 
{
  char __fpBuf[8];
  
  fingerprint_sendCommand(uart_number, CMD_GET_USERS_COUNT, 0, 0, 0);    
  fingerprint_readResponse(uart_number, __fpBuf);
  
  if (__fpBuf[2] == 0x00) return ((((int) __fpBuf[0]) << 8) | ((int) __fpBuf[1]));
  else return -1;  
}



int fingerprint_scan(uart_num uart_number, int userId, int *uid) 
{
  char __u2 = userId & 0xFF;
  char __u1 = (userId >> 8) & 0xFF;
  char __fpBuf[8];
  int idTemp = 0;
  *uid = 0;
  
  if(userId < 1) {
    fingerprint_sendCommand(uart_number, CMD_SCAN_COMPARE_1_TO_N, 0, 0, 0);    
    fingerprint_readResponse(uart_number, __fpBuf);
    *uid = ((((int) __fpBuf[0]) << 8) | ((int) __fpBuf[1]));
  } else {
    fingerprint_sendCommand(uart_number, CMD_SCAN_COMPARE_1_TO_1, __u1, __u2, 0);    
    fingerprint_readResponse(uart_number, __fpBuf);
    if(__fpBuf[2] == ACK_SUCCESS) {
      *uid = userId;
      fingerprint_sendCommand(uart_number, CMD_READ_USER_PRIVLAGE, __u1, __u2, 0);    
      fingerprint_readResponse(uart_number, __fpBuf);
    }      
  }
  
  return (int) __fpBuf[2];  
}



int fingerprint_lookupUserPrivlage(uart_num uart_number, int userId) 
{
  char __u2 = userId & 0xFF;
  char __u1 = (userId >> 8) & 0xFF;
  char __fpBuf[8];
  
  if(userId > 0) {
    fingerprint_sendCommand(uart_number, CMD_READ_USER_PRIVLAGE, __u1, __u2, 0);    
    fingerprint_readResponse(uart_number, __fpBuf);
  }
  
  return (int) __fpBuf[2];  
}  
  


int fingerprint_setTimeout(uart_num uart_number, int timeout) 
{
  char __fpBuf[8];
  unsigned char tto = (unsigned char) abs(timeout);   // convert roughly to milliseconds
    
  fingerprint_sendCommand(uart_number, CMD_SET_SCAN_TIMEOUT, 0, tto, 0);    
  fingerprint_readResponse(uart_number, __fpBuf);
  
  return (int) __fpBuf[2];  
}



int fingerprint_setStrictness(uart_num uart_number, char s_level) 
{
  char __fpBuf[8];
  if(s_level > 9) s_level = 9;
  if(s_level < 0) s_level = 0;

  fingerprint_sendCommand(uart_number, CMD_SENSITIVITY, 0, s_level, 0);    
  fingerprint_readResponse(uart_number, __fpBuf);
  
  return (int) __fpBuf[2];  
}


/**
 * TERMS OF USE: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
