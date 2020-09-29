/****************************************************************************
 * Project           	      : shakti devt board
 * Name of the file	      : gyro_iot.c
 * Brief Description of file  : reads real time calendar (date & time) over i2c interface.
 * Name of Author             : Kotteeswaran, Yasaswini
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
@file gyro_iot.c
@brief Contains the driver routines to configure and read DS3231.
@detail I2C based routines to configures and read the Accelerometer cum Gyroscope.
*/
#include<stdint.h>
#include<math.h>
#include<time.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>  
#include"defines.h"
#include"MPU6050.h"
#include"iot.h"
#define DEBUG
#define MPU6050_SLAVE_ADDRESS 0xD0
#define MPU_6050_OUTPUT_BEGIN 0x3B
#define MPU6050_DLPF_CFG 0x20
#define MPU6050_SMPL_DIV 0x19
#define PI 3.141592654
#define MAX 1024
char buff[MAX];
char readbuf[20];
int AccX,AccY,AccZ;
int accAngleX, accAngleY, GyroAngleX, GyroAngleY, GyroAngleZ;
int roll, pitch, yaw;
int AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
int AccErrorX,AccErrory;
int GyroX, GyroY, GyroZ;
int c = 0;
int i=0;
unsigned char delay = 100;
#ifdef TIMER
double elapsedTime;
#endif
#ifdef ETHERNET

unsigned char index=0;
int length;

int n;
typedef struct
{
   char AccXL;
   char AccXH;
   char AccYL;
   char AccYH;
   char AccZL;
   char AccZH;
   char GyroXL;
   char GyroXH;
   char GyroYL;
   char GyroYH;
   char GyroZL;
   char GyroZH;
} AccReadData;

AccReadData readGyroData[10];
void send_data(int sockfd) ;
#endif
#ifdef Wi-Fi
char uart0TxBuffer[2048];
int i,l=0,m=0,n=0;
char ssid[20],pwd[20],bssisd[40];
char *rec_buf;
int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
char checksum;
#endif

int I2c_Read_byte(unsigned char ,unsigned char );

/** @fn measuring_value
 * @brief 
 * @details 
 * @warning  
 */
void measuring_value()
{
// read accelerometer data
   I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_WRITE, delay);//selecting slave to be read
   I2cWriteData(MPU_6050_OUTPUT_BEGIN, delay);//selecting register to be read
   I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_READ, delay);
   I2c_shakti_readbytes(readbuf,14,1);//to read the output values 
  printf("helloworld inside measuring value\n");
  
   AccX = (readbuf[0]<<8 |readbuf[1]) / 4096; //16-bit X-axis data
   AccY = (readbuf[2]<<8 |readbuf[3]) / 4096; //16-bit Y-axis data
   AccZ = (readbuf[4]<<8 |readbuf[5]) / 4096; //16-bit Z-axis data
   
   GyroX = (readbuf[8]<<8 |readbuf[9]) /65.5 ; //
   GyroY = (readbuf[10]<<8 |readbuf[11]) / 65.5; //
   GyroZ = (readbuf[12]<<8 |readbuf[13]) / 65.5;

// Correct the outputs with the calculated error values
     GyroX = GyroX + GyroErrorX ; // GyroErrorX
     GyroY = GyroY - GyroErrorY; // GyroErrorY
     GyroZ = GyroZ + GyroErrorZ; // GyroErrorZ
}

/** @fn int main()
 * @brief Configures the MPU6050 and reads values.
 * @details Configures the MPU6050 and reads the accelerometer
 *          and gyrometer values.
 * @return int
 */
int main()
{
printf("MPU6050\n");
 I2cInit();//configuring data and clocks as output lines
   //VERIFYING DEVICE IDENTITY BY CHECKING "WHO AM I REGISTER"
printf("MPU6050 i2c Init completed\n");
	if(WHO_AM_I_DATA == I2c_Read_byte(MPU6050_SLAVE_ADDRESS,MPU6050_REG_WHO_AM_I) ){
	printf("WHO AM I REGISTER SUCCESFULLY VERIFIED");
   }
   else{
	printf("something went wrong , device identity not verified");
  }
   //configuring power management register/
   //RESETTING MPU6050 USING POWER NANAGEMENT REGISTER
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_REG_PWR_MGMT_1, MPU6050_RESET);

    delay_loop(1000,1000);   // we can use waitfor() function also (delay time to reset the device and initialisations proper)

  //setting internal clock of MPU6050
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_REG_PWR_MGMT_1, MPU6050_CLK_8MHz);

  // configuring accelerometer -set sensitivity scale factorMPU6050_SLAVE_ADDRESS to +-8g
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_REG_ACCEL_CONFIG, ACC_SENS_8g);

  // configuring gyroscope-set  sensitivity scale factor to +-500deg/sec
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_REG_GYRO_CONFIG,GYRO_SENS_500);
  // configuirng gyroscope output rate as 1khz
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_DLPF_CFG,0x01);
  // configuring sample divider to 99(i.e sample rate= gyroscope output rate /(1+sample divider))
    I2c_Write_byte(MPU6050_SLAVE_ADDRESS, MPU6050_SMPL_DIV, 0x63);//set sample rate to 10
   while (1){
   measuring_value();

#ifdef PRINT
   printf("AccErrorX: %d,AccErrMPU6050_SLAVE_ADDRESSorX");
   printf("AccErrorY: %d,AccErrorY");
   printf("GyroErrorX: %d,GyroErrorX");
   printf("GyroErrorY: %d,GyroErrorY");
   printf("GyroErrorZ %d,GyroErrorZ");
#endif
   }
#ifdef ETHERNET 
	send_data(sockfd);
	
    // socket create and varification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("socket creation failed...\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("192.168.6.111"); 
    servaddr.sin_port = htons(PORT); 
  
    // connect the client socket to server socket 
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 
    else
        printf("connected to the server..\n"); 
  
    // function for chat 
    send_data(sockfd); 
  
    // close the socket 
    close(sockfd); 
#endif

#ifdef Wi-Fi
AT: sprintf(uart0TxBuffer,"%s","AT\n");//Just to confirm the board connection. 
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2);
*rec_buf = UartReceiveString();
if(NULL=strstr(*rec_buf,"OK\n")) 
{
sprintf(uart0TxBuffer,"%s\n",*rec_buf);
printf("Error in connection\n");
goto AT;
}
else sprintf(uart0TxBuffer,"%s\n","*rec_buf)");

AT+MODE: sprintf(uart0TxBuffer,"%s","AT+MODE=3");//Setting esp32 in Station+SoftAP MODE
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2);
*rec_buf=UartReceiveString();
if(NULL=strstr(*rec_buf,"OK\n")) 
{
sprintf(uart0TxBuffer,"%s\n","*rec_buf");
printf("Error in connection\n");
goto AT+MODE;
}
else sprintf(uart0TxBuffer,"%s\n","*rec_buf");

AT+CWLAPOPT: sprintf(uart0TxBuffer,"%s","AT+CWLAPOPT=0,31\n");//Configuring list AP command //0==>APs are arranged in the order of RSSI, 31==>all mask bits are 1 i.e,all parameters are listed
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2);
*rec_buf=UartReceiveString();
if(NULL=strstr(*rec_buf,"OK\n")) 
{
sprintf(uart0TxBuffer,"%s\n",*rec_buf);
printf("Error in configuring of list ap command\n");
goto AT+CWLAPOPT;
}
else sprintf(uart0TxBuffer,"%s\n",*rec_buf);

/*sprintf(uart0TxBuffer,"%s","AT+CWLAP\n");//Listing available access points to which esp32 can be connected
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2)*/

sprintf(uart0TxBuffer,"%s\n","AT+CWJAP=\"ssid\",\"pwd\",\"bssid\"");//Connecting to a selected access point 
 //"NetworkName","NetworkPassword","MacAddress(optional)"
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2);
*rec_buf=UartReceiveString();
if(NULL=strstr(*rec_buf,"OK\n")){
	if(NULL=strstr(*rec_buf,"+CWJAP:1\n")) 
{
	printf("connection timeout\n");
	goto AT;
}						
	else if  (NULL=strstr(UartReceiveString(),"+CWJAP:2\n")) {
	printf("wrong password\n");
	goto AT+CWJAP;
}
	else if  (NULL=strstr(UartReceiveString(),"+CWJAP:3\n")) {
	printf("cannot find the target AP\n");
	goto AT+CWJAP;
}
	else if  (NULL=strstr(UartReceiveString(),"+CWJAP:4\n")) {
 	printf("connection failed\n");
	goto AT;
}
	else {
	printf("unknown error occured\n");
	goto AT;
}
}
else sprintf(uart0TxBuffer,"%s\n",*rec_buf);

sprintf(uart0TxBuffer,"%s","AT+CIPSTATUS\n");
UartTransmitString(uart0TxBuffer);
*rec_buf=UartReceiveString();
sprintf(uart0TxBuffer,"%s",*rec_buf);

sprintf(uart0TxBuffer,"%s","AT+CIFSR\n") //To confirm that esp32 is connected to wifi, we obtain its IP Address
UartTransmitString(uart0TxBuffer);
*rec_buf=UartReceiveString();
sprintf(uart0TxBuffer,"%s",*rec_buf);

AT+CIPSTART:sprintf(uart0TxBuffer,"AT+CIPSTART=\"TCP\",\"%s\",\"8080\"\n",*rec_buf);
UartTransmitString(uart0TxBuffer);
*rec_buf = UartReceiveString();
if (NULL = strstr(*rec_buf,"OK\n")) 
{
sprintf(uart0TxBuffer,"%s\n",*rec_buf);
printf("Error in connection\n");
goto AT;
}
else sprintf(uart0TxBuffer,"%s\n","*rec_buf)"); 

sprintf(uart0TxBuffer,"%s","AT+CIPSEND=191\n");
strcpy(uart0TxBuffer, buff);
UartTransmitString(uart0TxBuffer);
Delay(DELAY1,DELAY2);
#endif
 return 0;
}

/** @fn void calculate_imu_error()
 * @brief Calculates the IMU error.
 * @details Calculates the error in the read value.
 */
void calculate_imu_error()
{
   int c=0;
   int yaw;
   float accAngleX, accAngleY, GyroAngleX, GyroAngleY, GyroAngleZ;
   while (c < 200)
 {
   measuring_value();

   AccErrorX = AccErrorX + (atan(AccY / sqrt(pow(AccX, 2)) + pow(AccZ, 2)))* 180 / PI;
   AccErrorY = AccErrorY + (atan(AccY / sqrt(pow(AccX, 2)) + pow(AccZ, 2)))* 180 / PI;

    GyroErrorX = GyroErrorX + (GyroX / 65.5);
    GyroErrorY = GyroErrorY + (GyroY / 65.5);
    GyroErrorZ = GyroErrorZ + (GyroZ / 65.5);
    c++;
 }
   c=0;
   AccErrorX = AccErrorX / 200;
   AccErrorY = AccErrorY / 200;

  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorX; // AccErrorX 
  accAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) - AccErrorY; // AccErrory
  // sum all reading
    GyroErrorX = GyroErrorX / 200;
    GyroErrorY = GyroErrorY / 200;
    GyroErrorZ = GyroErrorZ / 200;

    GyroX = GyroX - GyroErrorX ; // GyroErrorX
    GyroY = GyroY - GyroErrorY; // GyroErrorY
    GyroZ = GyroZ - GyroErrorZ; // GyroErrorZ
}

#ifdef ETHERNET

/** @fn void send_data(int sockfd) 
 * @brief Send the data
 * @details Sends the data over ethernet link to the server.
 * @param int Socket id
 */
void send_data(int sockfd) 
{  
        for (;;) 
       { 
        bzero(buff, sizeof(buff)); 
       // printf("Enter the string : "); 
	/* sending data packets:
*structure is as follows*/

	buff[index++] = '$';
	buff[index++] = (CLI_SER << 7) | GYRO_SENSOR_ID; 
	buff[index++] = comm_header;
	buff[index++] = '[';
switch(comm_header)
{
        case '0':length = 191;
	case '1':
		for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].AccXL;
		buff[index++] = readGyroData[i].AccXH;
		buff[index++] = ',';
		}	
		buff[index++] = ']';	
		buff[index++] = '[';
		for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].AccYL;
		buff[index++] = readGyroData[i].AccYH;
		buff[index++] = ',';
		}	
		buff[index++] = ']';
		buff[index++] = '[';
		for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].AccZL;
		buff[index++] = readGyroData[i].AccZH;
		buff[index++] = ','
		}	
		buff[index++] = ']';
		if(comm_header=='1') 
			{       length = 98;
				break;
			}

        case '2': buff[index++] = '[';
		for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].GyroXL;
		buff[index++] = readGyroData[i].GyroXH;
		buff[index++] = ',';
		}	
		buff[index++] = ']';
		buff[index++] = '[';
        	for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].GyroYL;
		buff[index++] = readGyroData[i].GyroYH;
		buff[index++] = ',';
		}	
		buff[index++] = ']';

       		 for(i = 0; i < 10; i++)
		{
		buff[index++] = readGyroData[i].GyrOZL;
		buff[index++] = readGyroData[i].GyroZH;
		buff[index++] = ',';
		}	
		buff[index++] = ']';
		if(comm_header=='2') 
	       	{
			length = 98;
			break;
		}
}
		buff[index++] = '*';
		buff[index++] = 
				for (k=0 ;k< length-1;k++)
				{
					checksum= buff[k]^buff[k+1];
        n = 0; 
       // while ((buff[n++] = getchar()) != '\n') 
       //     ; 
        write(sockfd, buff,length); 
        //bzero(buff, sizeof(buff)); 
        read(sockfd, buff, sizeof(buff)); 
        printf("From Server : %s", buff); 
        if ((strncmp(buff, "exit", 4)) == 0) { 
            printf("Client Exit...\n"); 
            break; 
        } 
    } 
}
}
#endif

/** @fn  void I2c_Write_byte(unsigned char slave_address,unsigned char reg_address, unsigned char data )
 * @brief To write one byte of data into a particular register of a partivular slave
 * @details Writes one byte of data to the slave with given slave address and 
 * register address.
 * @param slave address
 * @param register address
 * @param data to be written
 */
 void I2c_Write_byte(unsigned char slave_address,unsigned char reg_address, unsigned char data )
{
    I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_WRITE, delay);
    I2cWriteData(reg_address, delay);
    I2cWriteData(data,delay);
    I2cStop(delay);
}

/** @fn int I2c_Read_byte(unsigned char slave_address,unsigned char reg_address)
 * @brief  To read one byte of data from a particular register of a partivular slave
 * @details Reads one byte of data by passing the slave address and the address of 
 * register that needs to be read.
 * @param slave address
 * @param register address
 * @return readdata
 */
int I2c_Read_byte(unsigned char slave_address,unsigned char reg_address)
{
   unsigned char readData;
   I2cSendSlaveAddress(slave_address, I2C_WRITE, delay);//selecting slave to be read
   I2cWriteData(reg_address, delay);//selecting register to be read
   I2cSendSlaveAddress(slave_address, I2C_READ, delay);
   readData = I2cReadDataAck(delay);
   I2cStop(delay);
   return (int) readData;
}

/** @fn int I2c_shakti_readbytes(char *buf, int count, int last)
 * @brief  To burst read (i.e read multiple bytes byte of data)
 * @details Reads n number of bytes from slave I2C device.
 * @param readbuf to store read data
 * @param count to tell how many bytes to read 
 * @param last
 * @return No. of values(bytes) read
 */
int I2c_shakti_readbytes(char *buf, int count, int last)
{
printf("start reading the slave device\n");
int i = 0;
#ifdef PCF8584
	for (i = 0; i <= count; i++) {
  	if (i != 0 && i < count){	
	readbuf[i]=I2cReadDataAck(delay);
	}
	else if(i==0) I2cReadDataAck(delay);//dummy read
	else {
	readbuf[i] = I2cReadDataNack(delay);
	}
/*The following loop is useful only  for eeprom*/
    if (last)  I2cStop(delay);
    else  I2cStart(delay);//sending repeated start
     }
return i-1; //excluding the dummy read	
#else 
	for (i = 0;i < count-1 ; i++) 
	readbuf[i]=I2cReadDataAck(delay);
	readbuf[i]=I2cReadDataNack(delay);

/*The following loop is useful only  for eeprom*/
if (last)
I2cStop(delay);
else
I2cStart(delay);//sending repeated start	
#endif
return i+1;
} 


