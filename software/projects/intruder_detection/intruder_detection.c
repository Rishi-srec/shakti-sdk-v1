/***************************************************************************
* Project				:  shakti devt board
* Name of the file			:  intruder_detection.c
* Brief Description of file		:  Intruder detection with keypad and PIR sensors.
* Name of Author			:  Sambhav Jain 
   
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
@file  intruder_detection.c  
@brief Intruder detection with keypad and PIR sensors.
@detail 
*/

/***********************************************************
*		Pin Definitions
************************************************************/
// GPIOx       - Pin Mapped
//     	0       -Keypad Top Line-4   	(Output)
//		1       -Keypad Top Line-3		(Output)
// 		2       -Keypad Top Line-2		(Output)
//		3       -Keypad Top Line-1		(Output)
//		4       -Keypad Bottom Line-4	(Output)
//		5       -Keypad Bottom Line-3	(Output)
//		6       -Keypad Bottom Line-2	(Output)
//		7       -Keypad Bottom Line-1	(Output)
//		Gnd		-Keypad Bottom Line-5	
//		Vcc		-Keypad Bottom Line-6	
 

//		8		-PIR Sensor Output Pin	(Input)
//		16		-LED P1					(Output)
//		19		-LED P2					(Output)
//		11		-Buzzer					(Output)
//		18		-LED P3					(Output)
//		21		-LED P4					(Output)
//		28		-Panic Button			(Input)
//		24		-Power Switch   		(Input)
//		25		-Deactivation Switch	(Input)

/***********************************************************
*		Include File Definitions
************************************************************/

#include "gpio.h"
#include "platform.h"
#include "intruder_detection.h"  //Includes the definitions//

/* global variable definition*/

int pir_present_state = 0;
int pir_previous_state = 0;

int panic_present_state=0;
int panic_previous_state=0;

int alarm_present_state=0;
int alarm_previous_state=0;

int first_flag = 1; 
int alarm_flag = 0;
int deactivate_flag = 0;
int alarm_counter_flag=1; //Default value 1
int keypad_password_flag=0;

int alarm_counter = 0;
int keypad_counter= 0;

// initialize key and colrow to -1 indicate no key is pressed
int key = -1;
int col_row = -1;

unsigned int row_pins[ROWNUM];
unsigned int col_pins[COLNUM];
unsigned int col[4]={0, 1, 2, 3};
unsigned int row[4]={4, 5, 6, 7};
int key_map[COLNUM][ROWNUM];
//Password
int pass_data[PASS_NUM];

//
int  keyTable[4][4] =
{
	{  1,  4,  7,  0},
	{  2,  5,  8, 15},
	{  3,  6,  9, 14},
	{ 10, 11, 12, 13}
};


/** @fn void write_gpio(unsigned long gpio_pin, int gpio_pin_mode )
 * @brief This function Writes to the Data Register 
 * @details 
 * @warning 
 * @param gpio_pin
 * @param gpio_pin_mode
 */
void write_gpio(unsigned long gpio_pin, int gpio_pin_mode )
{
	unsigned long read_data = 0;
	read_data = read_word(GPIO_DATA_REG);
	if(gpio_pin_mode == 1)
	{
		write_word (GPIO_DATA_REG, ( read_data | (gpio_pin ) ) );
	}
	else
	{
    	write_word(GPIO_DATA_REG, ( read_data & ( ~gpio_pin ) ) );  
  	}

}

/** @fn int pir_read_state(unsigned long temp_read_data)
 * @brief Reads the State of the PIR Sensor Output 
 * @details 
 * @warning 
 * @param Data Register
 * @return 
 */
int pir_read_state(unsigned long temp_read_data)
{
  pir_present_state= ((PIR_IN & temp_read_data) >>PIR_OFFSET);  
  if( (pir_present_state == 1) && (pir_previous_state == 0) ){
	//State of 0->1
   	//printf("\nHuman Detected - Intrusion Alert %x -- %x ",pir_present_state,pir_previous_state);
	pir_previous_state = pir_present_state;
    return 1;
  }
  else if((pir_present_state == 1) && (pir_previous_state == 1)) {
	//State of 1 -> 1
	 
	// printf("\nCode 2 %x -- %x ",pir_present_state,pir_previous_state);
	pir_previous_state = pir_present_state;
	return 2;
  }
  else {
	//State of 1->0 and 0->0
   	//printf("\n. %x -- %x",pir_present_state,pir_previous_state);
    pir_previous_state = pir_present_state;
    return 0;
  }
}


/** @fn int panic_button_read_state(unsigned long temp_read_data)
 * @brief Reads the State of the Panic Button 
 * @details 
 * @warning 
 * @param[in] Data Register
 * @return
 */
int panic_button_read_state(unsigned long temp_read_data)
{
  panic_present_state= ( (PANIC_BUTTON & temp_read_data) >> PANIC_BUTTON_OFFSET);  
  if( (panic_present_state == 1) && (panic_previous_state == 0) ) {
   // printf("\nHuman Detected - Intrusion Alert %x -- %x ",pir_present_state,pir_previous_state);
   // printf("\nSOS -Panic Button Activated");

	panic_previous_state = panic_present_state;
    return 1;
  }
  else if( (panic_previous_state == 1) && (panic_present_state == 1) ) {
	  //Function Returns 2 when mode is 1-1 (present and prev state)
	 // printf("\nCode 2 %x -- %x ",pir_present_state,pir_previous_state);
	panic_previous_state = panic_present_state;
	  return 2;
  }
  else {
   // printf("\n. %x -- %x",pir_present_state,pir_previous_state);
    panic_previous_state = panic_present_state;
    return 0;
  }
}


/** @fn void buzzer_tone()
 * @brief Outputs the Buzzer Tone
 * @details 
 * @warning 
 */

void buzzer_tone()
{
	int i=0;
	for (i = 0; i < 4; i++) {
		write_gpio(BUZZER,HIGH);	//Buzzer generates sound
		delay(1);
		write_gpio(BUZZER,LOW);		//No tone
		delay(1);
    }
}


/** @fn void  set_key_pins_map( unsigned int*  row, unsigned int* col, int table[COLNUM][ROWNUM] )
 * @brief This function sets the pins for the row and column
 * 			and maps the table to its keymap
 * @details 
 * @warning 	
 * @param
 * @param 
 */
void  set_key_pins_map( unsigned int*  row, unsigned int* col, int table[COLNUM][ROWNUM] )
{
	for(int i = 0 ; i < COLNUM ; i++)	{
		col_pins[i] = col[i]; // set col
	}
	for(int j = 0 ; j < ROWNUM ; j++) 	{
		row_pins[j] = row[j]; 	// set row
	}
	//Mapping table to its key map 
	for(int i = 0 ; i < COLNUM ; i++) {
		for(int j = 0 ; j < ROWNUM ; j++) {
			key_map[i][j] = table[i][j];
		}
	}
}

/** @fn int get_key(int col_row)
 * @brief This function returns the corresponding value in the keymap
 * @details 
 * @warning 
 * @param col, row 
 * @return 
 */
int get_key(int col_row)
{
    return key_map[col_row>>16][0xFFFF & col_row];
}

/** @fn get_col_row
 * @brief This function returns the column-row
 * @details Identifies the pressed key by making the corresponding coloumn
   low and reading the row values     
 * @warning  
 * @return 
 */
int get_col_row(void)
{
	int col_row = 0;
	unsigned long read_value = 0;
	unsigned long temp_read_data = 0;

	//Masking Data Reg 
	temp_read_data = read_word(GPIO_DATA_REG);
	write_word(GPIO_DATA_REG,( temp_read_data | (0xF << OUTPINS_OFFSET) ) );

	for(int i= 0 ; i < COLNUM ; i++) {
		temp_read_data = read_word(GPIO_DATA_REG);
		write_word(GPIO_DATA_REG, ( (0xF << OUTPINS_OFFSET)  &  (~(0x1 << (/* OUTPINS_OFFSET  +*/i) ) ) ) );

		for(int j = 0 ; j < ROWNUM ; j++) {
			read_value =  read_word(GPIO_DATA_REG);
			//printf("\n The read value is %lx; row_pins: %x", (read_value), row_pins[j]);
			if( ( read_value & (1 << row_pins[j] ) ) == 0 ) {
				//printf("\n The read value is %lx; [i:%d; j:%d] row_pins: %d; key_map=%d", read_value, i, j, row_pins[j], key_map[i][j]);
					return key_map[i][j];
			}
		}

		temp_read_data = read_word(GPIO_DATA_REG);
		write_word(GPIO_DATA_REG,(temp_read_data | (0xF << OUTPINS_OFFSET) ) );
	}

	return -1;
}

/** @fn void gpio_init()
 * @brief This maps the keypad from pins
 * @details    
 * @warning
 */
void gpio_init()
{
	set_key_pins_map(row, col,keyTable);	
	
	//Configuring the Pin mode for pins for keypad
	unsigned long read_data = 0;
	read_data = read_word(GPIO_DIRECTION_CNTRL_REG);
	write_word (GPIO_DIRECTION_CNTRL_REG, ( read_data | (OUTPINS ) ) );
	//Configuring the Pin mode for GPIO pins for other peripherals
	read_data = read_word(GPIO_DIRECTION_CNTRL_REG);
	//Configure direction of GPIO pins (1 ---> Output, 0 --> input)
	write_word(GPIO_DIRECTION_CNTRL_REG, ( read_data | GPIO_OUTPINS ) );

    //All the LED initially set to LOW
    write_gpio(LED_P1,LOW);
    write_gpio(LED_P2,LOW);
    write_gpio(LED_P3,LOW);
    write_gpio(LED_P4,LOW);
    write_gpio(BUZZER,LOW);
}

/** @fn int main()
 * @brief 
 * @details    
 * @warning
 * @return 
 */
int main()
{
	//Clearing the Data Register
	unsigned long read_data = 0;
	int prev_col_row = 0;
	printf("\n\n\tIntrusion Detection System");
	gpio_init(); //Intialization of gpio //

	printf("\nInitialising...");
	while(1)
	{	
		
		read_data = read_word(GPIO_DATA_REG);
		if(POWER_SWITCH & read_data) {
			if(first_flag){
				//First time giving the sensor to calibrate 
				delay(30);
				printf("\nIntruder System activated");
                first_flag=0;
                alarm_counter_flag =1;
			}
			//Intruder System - ON
			write_gpio(LED_P2,HIGH);
			//Deactivation of Alarm Switch
			if(DEACTIVATE_SWITCH & read_data) {
				deactivate_flag =1;
			}
			else {
				deactivate_flag =0;
			}
			

			int temp_read_PIR  = pir_read_state(read_data);
			int temp_read_panic = panic_button_read_state(read_data);
			alarm_present_state= ((temp_read_PIR > 0) || (temp_read_panic == 1) || alarm_flag == 1 ) && (deactivate_flag == 0);
			
			if(alarm_present_state == 1 && alarm_previous_state == 0) {
				//printf("\n%x   %x",alarm_previous_state,alarm_present_state);
				if(temp_read_PIR == 1)
					printf("\nHuman Detection\nDeactivate to stop Alarm");
				else if(temp_read_panic == 1)
					printf("\nPanic Button\nDeactivate to stop Alarm");
				//else {printf("\nDeactivate the Alarm to stop...");}

				write_gpio(LED_P1,HIGH);
				buzzer_tone();
				alarm_flag = 1;
				alarm_previous_state = alarm_present_state;

			}
			else if(alarm_present_state == 1 && alarm_previous_state == 1) {
				//printf("\n%x   %x",alarm_previous_state,alarm_present_state);
				write_gpio(LED_P1,HIGH);
				buzzer_tone();
				alarm_flag =1;
				alarm_previous_state = alarm_present_state;
                alarm_counter++;
                //printf("\n\t\t\t%x",alarm_counter);

                if(alarm_counter == ALARM_TIMER & ( alarm_counter_flag == 1 ) ) {
                    write_gpio(LED_P3,HIGH);
                    printf("\nContacting Police for Assistance");
                    
					alarm_counter_flag = 0;
                    alarm_counter = 0;
                }
                else if(alarm_counter == ALARM_TIMER) {
                    //Making Sure the alarm counter does not overflow
                    alarm_counter = 0;
                }

			}
			else if(alarm_present_state == 0 && alarm_previous_state == 1) {
				alarm_flag = 0;
				write_gpio(LED_P1,LOW);

                
                write_gpio(LED_P3,LOW);
                alarm_counter_flag = 1;
                alarm_counter= 0;
				
                printf("\nAlarm Succesfully devactivated successfully");
				alarm_previous_state = alarm_present_state;
				
			}
			else {
                //Flags
				alarm_flag = 0;
                alarm_counter= 0;
                alarm_counter_flag = 1;

                //write_gpio(LED_P3,LOW);
				write_gpio(LED_P1,LOW);
				alarm_previous_state = alarm_present_state ;
			}

			col_row = get_col_row();
			//KeyPad Sensor 
			if( col_row != -1 ) {
	//			key = get_key(col_row);
	#if 0
					if(prev_col_row != col_row)	
					{
	//			printf("\ncol: %d; row: %d is value: %d", col_row >> 16, (0xFFFF & col_row), key );
					prev_col_row = col_row;
					printf("\nThe %x Key Pressed", col_row);
					}
	#else
					printf("\nThe %x Key Pressed", col_row);
                    
					if( col_row == key_map[3][3]) {
						printf("\nEnter Password-");
					}
					delay(1);
	#endif
			}
		}
		else {	
			//Intruder System Off
			write_gpio(LED_P2,LOW);
			first_flag = 1;
		}
		//end of infinite while loop
	}
	return 0;
}
