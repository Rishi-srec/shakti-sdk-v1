/***************************************************************************
 * Project           		: shakti-sdk
 * Name of the file	     	: vectored.c
 * Brief Description of file    : source file for first level of vector
                                  based trap handling, redirecting to ISRs
 * Name of Author    	        : Prajwal Prakash
 * Email ID                     : prajwal.prakash2000@gmail.com

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
*@file vectored.c
*@brief source file for first level of vector based trap handling, redirecting to ISRs
*@details has wrapper functions to call relevant ISRs
*/

#include "traps.h"
#include "defines.h"
#include "vectored.h"
#include "log.h"
#include "platform.h"

/**
 * @fn enable_vectored
 * @brief Set up system to handle traps and interrupts in vectored mode
 * @details Set mtvec to vectored_trap_entry
 * @param none
 * @return none
*/
void enable_vectored(){
    log_trace("\nenable_vector entered\n");
    asm volatile("la t0, vectored_trap_entry\t \n"
                "csrw mtvec, t0\t \n"
                "li t0, 0x1\t \n"
                "csrs mtvec, t0\t \n");
    log_trace("\nenable_vector exited\n");
}


/** @fn m_t_wrapper
 * @brief Handles the machine mode timer interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int m_t_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nm_t_int entered\n");
    mcause_interrupt_table[MACH_TIMER_INTERRUPT](mcause, epc);
    //log_trace("\nm_t_int exited\n");
    return epc;
}

/** @fn m_s_wrapper
 * @brief Handles the machine mode software interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int m_s_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nm_s_int entered\n");
    mcause_interrupt_table[MACH_SW_INTERRUPT](mcause, epc);
    //log_trace("m_s_int exited\n");
    return epc;
}

/** @fn m_e_wrapper
 * @brief Handles the machine mode external interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int m_e_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nm_e_int entered\n");
    mcause_interrupt_table[MACH_EXTERNAL_INTERRUPT](mcause, epc);
    //log_trace("m_e_int exited\n");
    return epc;
}

/** @fn s_t_wrapper
 * @brief Handles the supervisor mode timer interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int s_t_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\ns_t_int entered\n");
    mcause_interrupt_table[SUPER_TIMER_INTERRUPT](mcause, epc);
    //log_trace("s_t_int exited\n");
    return epc;
}


/** @fn s_s_wrapper
 * @brief Handles the supervisor mode software interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int s_s_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\ns_s_int entered\n");
    mcause_interrupt_table[SUPER_SW_INTERRUPT](mcause, epc);
    //log_trace("s_s_int exited\n");
    return epc;
}

/** @fn s_e_wrapper
 * @brief Handles the supervisor mode external interrupt
 * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int s_e_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\ns_s_int entered\n");
    mcause_interrupt_table[SUPERVISOR_EXT_INTERRUPT](mcause, epc);
    //log_trace("s_s_int exited\n");
    return epc;
}


/** @fn u_t_int
 * @brief Handles the user mode timer interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int u_t_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nu_t_int entered\n");
    mcause_interrupt_table[USER_TIMER_INTERRUPT](mcause, epc);
    //log_trace("u_u_int exited\n");
    return epc;
}

/** @fn u_e_int
 * @brief Handles the user mode external interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int u_e_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nu_e_int entered\n");
    mcause_interrupt_table[USER_EXT_INTERRUPT](mcause, epc);
    //log_trace("u_e_int exited\n");
    return epc;
}

/** @fn res_0_int
 * @brief Handles the hypervisor mode software interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_0_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_0_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT0](mcause, epc);
    //log_trace("res_0_int exited\n");
    return epc;
}

/** @fn res_1_int
 * @brief Handles the hypervisor mode timer interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_1_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_1_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT1](mcause, epc);
    //log_trace("res_1_int exited\n");
    return epc;
}

/** @fn res_2_int
 * @brief Handles the hypervisor mode external interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_2_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_2_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT2](mcause, epc);
    //log_trace("res_2_int exited\n");
    return epc;
}

/** @fn res_3_int
 * @brief Handles the reserved 3 interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_3_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_3_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT3](mcause, epc);
    //log_trace("res_3_int exited\n");
    return epc;
}

/** @fn res_4_int
 * @brief Handles the reserved 4 interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_4_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_4_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT4](mcause, epc);
    //log_trace("res_4_int exited\n");
    return epc;
}

/** @fn res_5_int
 * @brief Handles the reserved 5 interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_5_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_5_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT5](mcause, epc);
    //log_trace("res_5_int exited\n");
    return epc;
}

/** @fn res_6_int
 * @brief Handles the reserved 6 interrupt
  * @details Calls the relevant interrupt service routine and returns epc value
 */
unsigned int res_6_wrapper(uintptr_t mcause, uintptr_t epc)
{
    //log_trace("\nres_6_int entered\n");
    mcause_interrupt_table[RESERVED_INTERRUPT6](mcause, epc);
    //log_trace("res_6_int exited\n");
    return epc;
}