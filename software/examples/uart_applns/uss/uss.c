/*****************************************************************************

 Project Name		: MDP - Microprocessor Development Project
 Project Code		: HD083D
 Created		: 07-Jul-2020
 Filename		: main.c
 Purpose		: Proximity sensor program
 Description		: Proximity sensor program using gpio
 Author(s)		: Premjith A V
 Email			: premjith@cdac.in
    
 See LICENSE for license details.
******************************************************************************/


#include "configs.h"
#include "uartv.h"
#include "gpiov.h"


/**
 @file ultrasound_sensor_pgm.c
 @brief Main program for ultrasound sensor application
*/

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

/** @fn main
 * @brief Proximity sensor program
 * @details Proximity sensor program
 * @warning 
 * @param[in] No input parameter first’
 * @param[Out] No output parameter 
*/
void main ()
{
	
	UC uart_error;	
	US pin_data1 = 0;
	UC repeat,rx = 0;
	clock_t start_time=0, end_time=0, total_time=0;
	UL distance=0;

		repeat = 0;


		printf("\n\r ***************************************************************************");
		printf("\n\r INFO: Connect ultrasound sensor [HC-SR04] to GPIO 0 ");	
		printf("\n\r INFO: Please try to put an object infront of ultrasound sensor");	
		printf("\n\r INFO: The distance will display below");	
		printf("\n\r ***************************************************************************\n\r\n\r");

		while(!repeat)
		{					
			GPIO_write_pin(0,LOW);
			GPIO_write_pin(0,HIGH);				
			udelay(10);
			GPIO_write_pin(0,LOW);	
			total_time = pulse_duration(1,HIGH);				
			distance = microsecondsToCentimeters(total_time);
			printf("\r Distance :%4d cm",distance);			
		
			udelay(6000);
		}
	
}




