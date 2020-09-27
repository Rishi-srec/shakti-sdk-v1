/*******************************************************************************
 * Name of the file              : vectored.S
 * Brief description             : ASM to set-up vectored interrupts
 * Name of Author                : Prajwal Prakash
 * Email id                      : prajwal.prakash2000@gmail.com

 Copyright (C) 2020  IIT Madras. All rights reserved.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You shouLREG have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.

********************************************************************************/
#include "defines.h"

.globl vectored_trap_entry
.p2align 2
vectored_trap_entry:
    j trap_entry
    nop
    j s_s_int
    nop
    j res_0_int
    nop
    j m_s_int
    nop
    j u_t_int
    nop
    j s_t_int
    nop
    j res_1_int
    nop
    j m_t_int
    nop
    j u_e_int
    nop
    j s_e_int
    nop
    j res_2_int
    nop
    j m_e_int
    nop
    j res_3_int
    nop
    j res_4_int
    nop
    j res_5_int
    nop
    j res_6_int
    nop


s_s_int: 
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal s_s_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

m_s_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal m_s_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

u_t_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal u_t_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

s_t_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal s_t_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

m_t_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal m_t_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

m_e_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal m_e_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

u_e_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal u_e_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

s_e_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal s_e_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_0_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_0_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_1_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_1_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_2_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_2_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_3_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_3_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_4_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_4_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_5_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_5_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret

res_6_int:
    addi sp, sp, -1 * REGSIZE
    SREG ra, 1* REGSIZE(sp)
    jal context_save
    
    csrr a0, mcause
    csrr a1, mepc
    mv a2, sp
    jal res_6_wrapper
    csrw mepc, a0

    jal context_restore
    LREG ra, 1 * REGSIZE(sp)
    addi sp, sp, 1 * REGSIZE
    mret


context_save:
    addi sp, sp, -31*REGSIZE
    #x0 is always 0
    #x1 = ra is already saved before this point
    SREG x2, 1*REGSIZE(sp)
    SREG x3, 2*REGSIZE(sp)
    SREG x4, 3*REGSIZE(sp)
    SREG x5, 4*REGSIZE(sp)
    SREG x6, 5*REGSIZE(sp)
    SREG x7, 6*REGSIZE(sp)
    SREG x8, 7*REGSIZE(sp)
    SREG x9, 8*REGSIZE(sp)
    SREG x10, 9*REGSIZE(sp)
    SREG x11, 10*REGSIZE(sp)
    SREG x12, 11*REGSIZE(sp)
    SREG x13, 12*REGSIZE(sp)
    SREG x14, 13*REGSIZE(sp)
    SREG x15, 14*REGSIZE(sp)
    SREG x16, 15*REGSIZE(sp)
    SREG x17, 16*REGSIZE(sp)
    SREG x18, 17*REGSIZE(sp)
    SREG x19, 18*REGSIZE(sp)
    SREG x20, 19*REGSIZE(sp)
    SREG x21, 20*REGSIZE(sp)
    SREG x22, 21*REGSIZE(sp)
    SREG x23, 22*REGSIZE(sp)
    SREG x24, 23*REGSIZE(sp)
    SREG x25, 24*REGSIZE(sp)
    SREG x26, 25*REGSIZE(sp)
    SREG x27, 26*REGSIZE(sp)
    SREG x28, 27*REGSIZE(sp)
    SREG x29, 28*REGSIZE(sp)
    SREG x30, 29*REGSIZE(sp)
    SREG x31, 30*REGSIZE(sp)
    ret


context_restore:

    LREG x2, 1*REGSIZE(sp)
    LREG x3, 2*REGSIZE(sp)
    LREG x4, 3*REGSIZE(sp)
    LREG x5, 4*REGSIZE(sp)
    LREG x6, 5*REGSIZE(sp)
    LREG x7, 6*REGSIZE(sp)
    LREG x8, 7*REGSIZE(sp)
    LREG x9, 8*REGSIZE(sp)
    LREG x10, 9*REGSIZE(sp)
    LREG x11, 10*REGSIZE(sp)
    LREG x12, 11*REGSIZE(sp)
    LREG x13, 12*REGSIZE(sp)
    LREG x14, 13*REGSIZE(sp)
    LREG x15, 14*REGSIZE(sp)
    LREG x16, 15*REGSIZE(sp)
    LREG x17, 16*REGSIZE(sp)
    LREG x18, 17*REGSIZE(sp)
    LREG x19, 18*REGSIZE(sp)
    LREG x20, 19*REGSIZE(sp)
    LREG x21, 20*REGSIZE(sp)
    LREG x22, 21*REGSIZE(sp)
    LREG x23, 22*REGSIZE(sp)
    LREG x24, 23*REGSIZE(sp)
    LREG x25, 24*REGSIZE(sp)
    LREG x26, 25*REGSIZE(sp)
    LREG x27, 26*REGSIZE(sp)
    LREG x28, 27*REGSIZE(sp)
    LREG x29, 28*REGSIZE(sp)
    LREG x30, 29*REGSIZE(sp)
    LREG x31, 30*REGSIZE(sp)
    addi sp, sp, 31 * REGSIZE
    #original ra saved on top of stack
    ret
