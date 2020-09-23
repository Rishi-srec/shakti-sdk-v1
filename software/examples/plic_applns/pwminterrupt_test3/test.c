/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  btnled.c
 * Brief Description of file    :  Controls the led operation with help of button,gpio based.
 * Name of Author    	        :  Sathya Narayanan N & Abhinav Ramnath
 * Email ID                     :  sathya281@gmail.com  & abhinavramnath13@gmail.com 
 
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
@file pwminterrupt
@brief A application to demonstrate working of pwm interrupts
@detail This file contains an application to demonstrate the working of pwm
 interrupts. The interrupts are enabled for a pwm module. The pwm module is
 confiured for interrupt mode and pwm registers are setup appropriately.
 Once the pwm timer overflows, an interrupt is generated and it is handled
 by the isr.
*/

#include "gpio.h"
#include "uart.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "pwm_driver.h"

/** @fn void dump_word_memory(unsigned int* start, unsigned int word_length)
 * @brief 
 * @details 
 * @warning
 * @param unsigned int* start
 * @param unsigned int word_length 
 */
void dump_word_memory(unsigned int* start, unsigned int word_length)
{
	unsigned int i=0;
	void *address;

	address = (unsigned int *) start;

	while(i++< word_length)
	{
		log_info("address = %x data = %x\n", address, *(unsigned int *) address);
		address+=4;
	}
}

/** @fn void dump_byte_memory(unsigned int* start, unsigned int word_length) 
 * @brief 
 * @details 
 * @warning 
 * @param unsigned int* start 
 * @param unsigned int word_length 
 */
void dump_byte_memory(unsigned int* start, unsigned int word_length)
{
	unsigned int i=0;
	void *address;

	address = (unsigned char *) start;

	while(i++< word_length)
	{
		log_info("address = %x data = %x\n", address, *(unsigned char
							       *) address);
		address+=1;
	}
}

/** @fn unsigned handle_button_press (unsigned num)
 * @brief 
 * @details 
 * @warning 
 * @param unsigned num 
 * @return unsigned 
 */
unsigned handle_button_press (unsigned num)
{
	log_info("button pressed\n");
	return 0;
}

/** @fn int main(void)
 * @brief 
 * @details 
 * @warning  
 * @return int 
 */
int main(void){

	unsigned int int_id = 1;
	register uint32_t retval;
	int i;
	printf("handle button press isr set\n");

	isr_table[PLIC_INTERRUPT_1] = handle_button_press;

	dump_word_memory(0x0c000000, 24);
	dump_byte_memory(0x0c001000, 4);
	dump_byte_memory(0x0c002000, 4);
	read_word(0x0c010000);
	read_word(0x0c010010);

	plic_init();

	//interrupt id 13, 10, 14, 20, 21 are coming continuously without manual trigger

	for(int i=1;i<7;i++)
		configure_interrupt(i);


	dump_word_memory(0x0c000000, 24);
	dump_byte_memory(0x0c001000, 4);
	dump_byte_memory(0x0c002000, 4);
	read_word(0x0c010000);
	read_word(0x0c010010);

#if 1
	// Enable Global (PLIC) interrupts.
	asm volatile("li      t0, 8\t\n"
		     "csrrs   zero, mstatus, t0\t\n"
		    );

	asm volatile("li      t0, 0x800\t\n"
		     "csrrs   zero, mie, t0\t\n"
		    );

	asm volatile(
		     "csrr %[retval], mstatus\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);
	asm volatile(
		     "csrr %[retval], mie\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);

	asm volatile(
		     "csrr %[retval], mip\n"
		     :
		     [retval]
		     "=r"
		     (retval)
		    );

	printf(" retval = %x\n", retval);
/*uncomment to test PWM 0 interrupts (interrupt ID - 5)
	pwm_configure(PWM_0,0xffff,0xff,0x01,false);
	pwm_start(PWM_0,2);
*/
/*uncomment to test PWM 1 interrupts (interrupt ID - 4)
	pwm_configure(PWM_1,0xffff,0xff,0x01,false);
	pwm_start(PWM_1,2);
*/
//uncomment to test PWM 2 interrupts (interrupt ID - 3)
	pwm_configure(PWM_2,0xffff,0xff,0x01,false); 
	pwm_start(PWM_2,2);

/*uncomment to test PWM 3 interrupts (interrupt ID - 2)
	pwm_configure(PWM_3,0xffff,0xff,0x01,false);
	pwm_start(PWM_3,2);
*/
/*uncomment to test PWM 4 interrupts (interrupt ID - 1)
	pwm_configure(PWM_4,0xffff,0xff,0x01,false);
	pwm_start(PWM_4,2);	
*/
/* test PWM 5 interrupts (Not working)
	pwm_configure(PWM_5,0xffff,0xff,0x01,false);
	pwm_start(PWM_5,2);
*/	
	while(1){
		asm volatile(
			     "csrr %[retval], mip\n"
			     :
			     [retval]
			     "=r"
			     (retval)
			    );

		i++;
		if((i%10000000) == 0){
			printf(" retval = %x\n", retval);
			dump_word_memory(0x0c000000, 24);
			dump_byte_memory(0x0c001000, 4);
			dump_byte_memory(0x0c002000, 4);
			read_word(0x0c010000);
			read_word(0x0c010010);
		}
	dump_byte_memory(0x00030008, 1);
	}
#endif

	return 0;
}

