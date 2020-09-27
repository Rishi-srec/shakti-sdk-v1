/***************************************************************************
* Project           			:  Learn with Shakti
* Name of the file	     		:  mtime_trap.S
* Created date			        :  
* Brief Description of file             :  Timer Interrupt Handling. 
* Name of Author    	                :  
* mail ID                               :  

    Copyright (C) 2020  IIT Madras. All rights reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You shoulw have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

***************************************************************************/
# set the mtvec lsb to 1. vector trap will be enabled.
# on mtime > mtimecmp, auto matically timer interrupt will happen
# hardware jumps to base + 4*mcause address location (only for interrupts)


# Below are address of mtime and mtimecmp registers in spike simulator
#define mtime  0x200bff8
#define mtimecmp 0x2004000


_start:	
#intialize t4,t3,t2,t6 to 0
andi t4,t4,0
andi t2,t2,0
andi t3,t3,0
andi t6,t6,0

#resetting mtip bit to zero. pending timer interrupt is cleared.
# so that any earlier timer interrupt is handled well
li t6,mtimecmp    # t6 has mtimecmp reg location
andi t3,t3,0
addi t3,t3,-1 #reg t6 set to -1
sw t3, 0(t6) # just increment the mtimecmp.

lui sp, 0x10011 #setting sp

#setting up vector trap mode
la t0, vtrap_entry 	# load temp register t0 with vtrap_entry
csrw mtvec,t0 		# write mtve with t0 value. This will set mtvec to trap_entry
li t1, 0x1  		#setting lsb to 1
csrs mtvec,t1 		#enable vector based interrupt, by setting lsb of mtvec to 1

#setting up timer interrupt

#step 1
#we are doing mtimecmp = mtime + delta
#delta is a time period after which we need an timer interrupt

li t6,mtimecmp    # t6 has mtimecmp reg location

# reg t4 has the delta value
#slli t4, t4,63
lui t4, 0x50000
addi t4, t4, 999

li t2,mtime      #reg t2 has mtime location, mtime is a 64 bit reg
lw t3, 0(t2)    # reg t3 has mtime reg value

add t3,t4,t3   # add delta value to reg t3
sw t3, 0(t6)   # store t3 value in mtimecmp reg location

#store 0 in upper 32 bit of mtimecmp
andi t3,t3,0
#addi t3,t3,1
sw t3, 4(t6)  # store 0 in mtimecmp + 4


#step 2
#enabling interrupts
li      t0, 0x80
csrrs   zero, mie, t0  # enable machine time interrupt
li      t0, 3
csrrs   zero, mstatus, t0 #enable global interrupt (MIE bit)

 

here: j here   # infinite loop


#this code has been suitably modified from trap_entry, for vectored trap.

.p2align 2
vtrap_entry:     # vtrap_entry has a nop to make sure, handlers are 4 byte aligned 
j u_sw_int_handler
nop
j s_sw_int_handler
nop
j h_sw_int_handler
nop
j m_sw_int_handler
nop
j u_ti_int_handler
nop
j s_ti_int_handler
nop
j h_ti_int_handler
nop
j m_ti_int_handler #machine mode timer interrupt handler. H/W jumps here, if machine timer interrupt happens
nop #nop is added here. so that each handler is at locations that are 4 byte aligned, starting from vtrap_entry
j u_ex_int_handler
nop
j s_ex_int_handler
nop
j h_exint_handler
nop
j m_ex_int_handler
nop

u_sw_int_handler:
j here

s_sw_int_handler:
j here

h_sw_int_handler:
j here

m_sw_int_handler:
j here

u_ti_int_handler:
j here

s_ti_int_handler:
j here

h_ti_int_handler:
j here

m_ti_int_handler:

#please add appln specific actions here

#clearing mtip bit, so that next timer interrupt comes for processing. mandatory step
li t6,mtimecmp    # t6 has mtimecmp reg location
andi t3,t3,0
addi t3,t3,-1 # writing -1 to reg t3
sw t3, 0(t6) # just increment the mtimecmp + 4 location value by 1.
j here

u_ex_int_handler:
j here

s_ex_int_handler:
j here

h_exint_handler:
j here

m_ex_int_handler:
j here

#below code unused
_data1:
.word 	7
.word 	6
.word 	5
.word 	4


#trap handler
trap_handler:
#store mepc and mcause
#mcause tells why the trap is
csrr s11, mcause
ebreak
#mepc tells where the trap is
ret

.p2align 2
trap_entry:     # currently trap_entry has a nop. Later we will add actual instrns
addi sp, sp, -32*8
 #x0 is always 0
sw x1, 1*8(sp)
sw x2, 2*8(sp)
sw x3, 3*8(sp)
sw x4, 4*8(sp)
sw x5, 5*8(sp)
sw x6, 6*8(sp)
sw x7, 7*8(sp)
sw x8, 8*8(sp)
sw x9, 9*8(sp)
sw x10, 10*8(sp)
sw x11, 11*8(sp)
sw x12, 12*8(sp)
sw x13, 13*8(sp)
sw x14, 14*8(sp)
sw x15, 15*8(sp)
sw x16, 16*8(sp)
sw x17, 17*8(sp)
sw x18, 18*8(sp)
sw x19, 19*8(sp)
sw x20, 20*8(sp)
sw x21, 21*8(sp)
sw x22, 22*8(sp)
sw x23, 23*8(sp)
sw x24, 24*8(sp)
sw x25, 25*8(sp)
sw x26, 26*8(sp)
sw x27, 27*8(sp)
sw x28, 28*8(sp)
sw x29, 29*8(sp)
sw x30, 30*8(sp)
sw x31, 31*8(sp)

jal trap_handler

lw x1, 1*8(sp)
lw x2, 2*8(sp)
lw x3, 3*8(sp)
lw x4, 4*8(sp)
lw x5, 5*8(sp)
lw x6, 6*8(sp)
lw x7, 7*8(sp)
lw x8, 8*8(sp)
lw x9, 9*8(sp)
lw x10, 10*8(sp)
lw x11, 11*8(sp)
lw x12, 12*8(sp)
lw x13, 13*8(sp)
lw x14, 14*8(sp)
lw x15, 15*8(sp)
lw x16, 16*8(sp)
lw x17, 17*8(sp)
lw x18, 18*8(sp)
lw x19, 19*8(sp)
lw x20, 20*8(sp)
lw x21, 21*8(sp)
lw x22, 22*8(sp)
lw x23, 23*8(sp)
lw x24, 24*8(sp)
lw x25, 25*8(sp)
lw x26, 26*8(sp)
lw x27, 27*8(sp)
lw x28, 28*8(sp)
lw x29, 29*8(sp)
lw x30, 30*8(sp)
lw x31, 31*8(sp)

addi sp, sp, 32*8
mret



