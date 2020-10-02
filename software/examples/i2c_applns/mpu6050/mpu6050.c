/*************************************************************************
 * Project           	       : shakti devt board
 * Name of the file	       : mpu6050.c
 * Brief Description of file   : Contains the driver routines to configure and read mpu6050.
 * Name of Author              : Kotteeswaran
 * Email ID                    : kottee.1@gmail.com

 Copyright (C) 2020  IIT Madras. All rights reserved.

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
@file   mpu6050.c
@brief  Contains the driver routines to write and read all locations of mpu6050.
@detail I2C based routines to write and read all locations of mpu6050.
*/

#include <stdint.h>//Includes the definitions of standard input/output functions//
#include "i2c.h"
#include "log.h"
#include "uart.h"

#define DELAY_VALUE 900
#define PRESCALER_COUNT 0x1F
#define SCLK_COUNT 0x91
#define I2C i2c_instance[1]

#define MPU6050_SLAVE_ADDRESS 0xD0
#define MPU_6050_OUTPUT_BEGIN 0x3B
#define MPU6050_DLPF_CFG 0x20
#define MPU6050_SMPL_DIV 0x19
#define MPU6050_ADDRESS   0x68
#define DEV_IDENTITY  0X68
#define DEV_IDENTITY_DATA_REG 0X75
#define MPU6050_REG_PWR_MGMT 0X6B
#define MPU6050_RESET         0x80
#define MPU6050_ACC_SENS_8G         0x10
#define MPU6050_REG_GYRO_CONFIG      0x1B // Gyroscope Configuration
#define MPU6050_REG_ACCEL_CONFIG     0x1C // Accelerometer Configuration
#define MPU6050_GYRO_SENS_500D       0x08

#define PI 3.141592654

char readbuf[20];
int AccX,AccY,AccZ;
int accAngleX, accAngleY, GyroAngleX, GyroAngleY, GyroAngleZ;
int roll, pitch, yaw;
int AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
int AccErrorX,AccErrory;
int GyroX, GyroY, GyroZ;
int c = 0;
int i=0;

#if 1
/** @fn int read_mpu6050_registers(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned int *readTemp, unsigned char length, unsigned long delay)
 * @brief Reads the ADC value from PCF8591
 * @details Reads 4 ADC values from PCF8591 over I2C interface
 * @param i2c_struct*
 * @param reg_offset
 * @param *readTemp
 * @param length
 * @param delay
 * @return read status (zero on success)
 */
int read_mpu6050_register(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned char *readTemp, unsigned char length, unsigned long delay)
{
	unsigned char read_buf[4] = {'\0'};
	int i = 0, j = 0,  k = 0, status=0;
	unsigned char temp = 0;

//Writes the slave address for write
	i2c_send_slave_address(i2c_instance, MPU6050_SLAVE_ADDRESS, I2C_WRITE, 800);
//Writes the pointer to address that needs to be read
	i2c_write_data(i2c_instance, reg_offset, delay);
//Stops the I2C transaction to start reading the temperature value.
	i2c_instance->control = I2C_STOP;
//	i2c_instance->control = I2C_REPSTART;

//Writes the slave address for read
	i2c_send_slave_address(i2c_instance, MPU6050_SLAVE_ADDRESS, I2C_READ, 800);

/* Make a dummy read as per spec of the I2C controller */
	i2c_instance->control = I2C_NACK;
	i2c_read_data(i2c_instance, &temp, delay);
	printf("\n Reading the data");

//Reads the MSB Byte of temperature [D9 - D1]
//	i2c_read_data(i2c_instance, &temp, delay);
	i2c_read_data_nack(i2c_instance, &temp, delay);
	printf("\n Reading the data from reg");

	*readTemp = temp;
	printf("\n Sending stop request");
	i2c_instance->control = I2C_STOP;

	return 0;
}

/** @fn int read_mpu6050_registers(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned int *readTemp, unsigned char length, unsigned long delay)
 * @brief Reads the ADC value from PCF8591
 * @details Reads 4 ADC values from PCF8591 over I2C interface
 * @param i2c_struct*
 * @param reg_offset
 * @param *readTemp
 * @param length
 * @param delay
 * @return read status (zero on success)
 */
int read_mpu6050_registers(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned int *readTemp, unsigned char length, unsigned long delay)
{
	unsigned char read_buf[4] = {'\0'};
	int i = 0, j = 0,  k = 0, status=0;
	unsigned char temp = 0;

//Writes the slave address for write
	i2c_send_slave_address(i2c_instance, MPU6050_SLAVE_ADDRESS, I2C_WRITE, 800);
//Writes the pointer to address that needs to be read
	i2c_write_data(i2c_instance, reg_offset, delay);
//Stops the I2C transaction to start reading the temperature value.
//	i2c_instance->control = I2C_STOP;
	i2c_instance->control = I2C_REPSTART;

//Writes the slave address for read
	i2c_send_slave_address(i2c_instance, MPU6050_SLAVE_ADDRESS, I2C_READ, 800);

/* Make a dummy read as per spec of the I2C controller */
	i2c_read_data(i2c_instance, &temp, delay);

//Reads the MSB Byte of temperature [D9 - D1]
	for(i = 0; i < length; i++)
	{
		i2c_read_data(i2c_instance, &temp, delay);
		*readTemp = temp;

		if(i == (length - 2))
		i2c_instance->control = I2C_NACK;
		*readTemp++;
	}
	
	i2c_instance->control = I2C_STOP;
	return 0;
}
#endif

/** @fn int write_pcf8591_registers(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned int *write_value, unsigned char length, unsigned long delay)
 * @brief WRites into PCF8591 Register
 * @details WRites the passed value into passed PCF8591 Register (address) over I2C interface.
 * @param i2c_struct*
 * @param reg_offset
 * @param write_value
 * @param length
 * @param delay
 * @return Write status (Zero on success)
 */
int write_mpu6050_registers(i2c_struct * i2c_instance, unsigned int reg_offset, unsigned int *write_value, unsigned char length, unsigned long delay)
{
	int i = 0, j = 0,  k = 0, status=0;
	unsigned int temp = 0;
	
	i2c_send_slave_address(i2c_instance, MPU6050_SLAVE_ADDRESS, I2C_WRITE, delay);
	i2c_write_data(i2c_instance, reg_offset, delay);
	delay_loop(200,200);
	
	if(1 == length)
	{
		i2c_write_data(i2c_instance,  ( *write_value	& 0xff), delay);
	}
	else
	{
		for(i = 0; i < length; i++)
		{
			i2c_write_data(i2c_instance, (*write_value++ & 0xff), delay);

		}
	}

//Stops the I2C transaction to start reading the temperature value.
	i2c_instance->control = I2C_STOP;
	return 0;
}

/*
   soft float library need to be added, if hardfloat not there
 */
void measuring_value(unsigned long delay)
{
	// read accelerometer data
#if 0
	I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_WRITE, delay);//selecting slave to be read
	I2cWriteData(MPU_6050_OUTPUT_BEGIN, delay);//selecting register to be read
	I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_READ, delay);
	i2c_dataread(readbuf,14,1, delay);//to read the output values
	printf("helloworld inside measuring value\n");
#else
	read_mpu6050_registers(I2C, MPU_6050_OUTPUT_BEGIN, readbuf, 14, delay);
	printf("\n Read Buf: { ");
	for(i = 0; i < 14; i++)
	{
		printf(" %x, ", readbuf[i] );
		}
	printf(" }\n" ); 
#endif
/*	AccX = (readbuf[0]<<8 |readbuf[1]) / 4096; //16-bit X-axis data
	AccY = (readbuf[2]<<8 |readbuf[3]) / 4096; //16-bit Y-axis data
	AccZ = (readbuf[4]<<8 |readbuf[5]) / 4096; //16-bit Z-axis data

	GyroX = (readbuf[8]<<8 |readbuf[9]) /65.5 ; //
	GyroY = (readbuf[10]<<8 |readbuf[11]) / 65.5; //
	GyroZ = (readbuf[12]<<8 |readbuf[13]) / 65.5;

	// Correct the outputs with the calculated error values
	GyroX = GyroX + GyroErrorX ; // GyroErrorX
	GyroY = GyroY - GyroErrorY; // GyroErrorY
	GyroZ = GyroZ + GyroErrorZ; // GyroErrorZ
*/
}


void calculate_imu_error(unsigned long delay)
{
	int c=0;
	int yaw;
	float accAngleX, accAngleY, GyroAngleX, GyroAngleY, GyroAngleZ;

	while(c<200)
	{
		measuring_value(delay);

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

unsigned char CheckForDeviceId(unsigned long delay)
{
	unsigned char readBuf = 0xFF;
#if 0
	I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_WRITE, delay);//selecting slave to be read
	SendAddressToReadOrWrite(DEV_IDENTITY_DATA_REG);
	i2c_stop();
	I2cSendSlaveAddress(MPU6050_SLAVE_ADDRESS, I2C_READ, delay);//selecting slave to be read
	i2c_byteread(&readBuf);
//	i2c_stop();
#else
	read_mpu6050_register(I2C, DEV_IDENTITY_DATA_REG, &readBuf, 1, delay);
#endif
	printf("\n Read Buf Value: %x", readBuf);
	return readBuf;
}

int main()
{
	unsigned char i = 0;
	int timeout;
	unsigned int tempReadValue = 0;
	unsigned long delay = 1000;
	unsigned int write_buf[7] = {0x00}, read_buf[7] = {0x00};
	unsigned char length = 0;

	log_debug("\tI2C: MPU6050 - ACC test\n");


	i2c_init();

	if(config_i2c(I2C, PRESCALER_COUNT, SCLK_COUNT))
	{
			log_error("\tSomething Wrong In Initialization\n");
			return -1;
	}
	else
		log_info("\tIntilization Happened Fine\n");

	printf("MPU6050 i2c Init completed\n");

	if(DEV_IDENTITY == CheckForDeviceId(delay))
	{
		printf("Device ID Check Success");
	}
	else
	{
		printf("Could not verify Device identity");
	}
	delay_loop(1000,1000);   // we can use waitfor() function also (delay time to reset the device and initialisations proper)
	//configuring power management register
	printf("\n Reset MPU");
	write_buf[0] = MPU6050_RESET;
	write_mpu6050_registers(I2C, MPU6050_REG_PWR_MGMT, &write_buf[0], 1,  delay);

	return 0;

	//setting internal clock of MPU6050
	write_buf[0] = 0x00;
	write_mpu6050_registers(I2C, MPU6050_REG_PWR_MGMT, &write_buf[0],  1, delay);


	printf("\n Configure Accelerometer");
	// configuring accelerometer -set sensitivity scale factor +-8g
	write_buf[0] = MPU6050_ACC_SENS_8G;
	write_mpu6050_registers(I2C, MPU6050_REG_ACCEL_CONFIG, &write_buf[0],  1, delay);

	printf("\n Configure Gyro");
	// configuring gyroscope-set  sensitivity scale factor to +-500deg/sec
	write_buf[0] = MPU6050_GYRO_SENS_500D;
	write_mpu6050_registers(I2C, MPU6050_REG_GYRO_CONFIG, &write_buf[0], 1, delay);


	printf("\n Configure Gyro sample rate");
	// configuirng gyroscope output rate as 1khz
	write_buf[0] = 0x01;
	write_mpu6050_registers(I2C, MPU6050_DLPF_CFG, &write_buf[0],  1, delay);

	printf("\n Configure sample divider");
	// configuring sample divider to 99(i.e sample rate= gyroscope output rate /(1+sample divider))
	write_buf[0] = 0x63;
	write_mpu6050_registers(I2C, MPU6050_SMPL_DIV, &write_buf[0],  1, delay);//set sample rate to 10

	printf("\n Entering While loop");

	while(1)
	{
		measuring_value(delay);
/*
		printf("AccErrorX: %d,AccErrMPU6050_SLAVE_ADDRESSorX");
		printf("AccErrorY: %d,AccErrorY");
		printf("GyroErrorX: %d,GyroErrorX");
		printf("GyroErrorY: %d,GyroErrorY");
		printf("GyroErrorZ %d,GyroErrorZ");
*/
		delay_loop(2000, 1000);
	}

	return 0;
}
