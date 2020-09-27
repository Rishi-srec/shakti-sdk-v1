/***************************************************************************
 * Project           			: shakti devt board
 * Name of the file	     		: vectored_test.c
 * Brief Description of file    :  
 * Name of Author    	        : Prajwal Prakash
 * Email ID                     : prajwal.prakash2000@gmail.com

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

#include "gpio.h"
#include "uart.h"
#include "traps.h"
#include "platform.h"
#include "plic_driver.h"
#include "log.h"
#include "clint_driver.h"
#include "defines.h"
#include "memory.h"
#include "pwm_driver.h"
#include "vectored.h"


int g = 1;
/** @fn clock_reset
 * @brief ISR to display regular time intervals, follows ISR signature
 * @param[in] uintptr_t, uintptr_t
 * @param[Out] none
 */
void clock_reset(uintptr_t int_id, uintptr_t epc){
	uint64_t value = 500000;
	if(g == 1){
		printf("Beginning clock");
	}
	else{
		printf(".");
	}
	g++;
	if(g % 10 == 0){
		printf("\n15 seconds\n");
	}
	*mtimecmp = (uint64_t) -1;
	*mtimecmp = *mtime + value;
}

/** @fn test_timer 
 * @brief test working of clint features with vectored mode
 * @param[in] none
 * @param[Out] int
 */
int test_timer(void){

	uint64_t value = 50000000;
	configure_counter(value);
	//enable_vectored();
	mcause_interrupt_table[MACH_TIMER_INTERRUPT] = clock_reset;

	asm volatile("li      t0, 0x80\t\n"
		     "csrrs   zero, mie, t0\t\n"
		    );
	printf("ENabling interupts - ");
	
	uint64_t id = 11;
	asm volatile(
		     "csrr %[id], mstatus\n"
		     :
		     [id]
		     "=r"
		     (id)
		    );

	log_info("mstatus value = %x\n",id);
	asm volatile("li      t0, 8\t\n"
		     "csrrs   zero, mstatus, t0\t\n"
		    );
	printf("Interrupts enabled ");
	asm volatile(
		     "csrr %[id], mstatus\n"
		     :
		     [id]
		     "=r"
		     (id)
		    );

	log_info("mstatus value = %x\n",id);
	while(1);

	return 0;

}

int main(void){

	test_timer();
	return 0;
}
