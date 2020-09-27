/*
All-in-one code to test PLIC interrupt on a vectored interrupt
*/

#if __riscv_xlen == 64
#define LREG ld
#define SREG sd
#define REGBYTES 8
#else
#define LREG lw
#define SREG sw
#define REGBYTES 4
#endif
#define PLIC_BASE_ADDR  0x0c000000
#define PLIC_ENABLE_BASE 0x0c002000
#define PLIC_THRESHOLD_ADDR 0x0c010000
#define PLIC_CLAIM_ADDR 0x0c010010
#define UART_BASE 0x00011300


_start:	
	andi sp,x0,0
	andi t1,x0,0
	andi t2,x0,0
	andi t3,x0,0
	andi t4,x0,0
	andi t5,x0,0
	andi t6,x0,0
	andi t0,x0,0
	andi a0,x0,0
	andi a1,x0,0
	andi a2,x0,0
	andi a3,x0,0
	andi a4,x0,0
	andi a5,x0,0
	andi a6,x0,0
	andi a7,x0,0
	andi s1,x0,0
	andi s2,x0,0
	andi s3,x0,0
	andi s4,x0,0
	andi s5,x0,0
	andi s6,x0,0
	andi s7,x0,0
	andi s8,x0,0
	andi s9,x0,0
	andi s10,x0,0
	andi s11,x0,0
	andi ra,x0,0
	andi gp,x0,0
	andi tp,x0,0
	andi s0,x0,0

	li sp, 0x80010000
	la t0, vectored_trap_entry
	csrw mtvec, t0              #trying to access mtvec
    li t0, 0x1
    csrs mtvec, t0
    lw t1, 0(t2)

	li t2, UART_BASE

.p2align 2
uart_status_simple:	
    lb  t1, 12(t2)
	andi t1,t1,0x2
	bnez t1, uart_status_simple
	andi t1,t1,0
	addi t1,t1,65
	sb t1, 4(t2)
	jal ra,init_interrupt	#initialize interrupt
loop:
j loop

init_interrupt:
	// Enable Global (PLIC) interrupts.
	andi t1,t1,0
	addi  t0,t1,0x8
	csrrs    t1, mstatus, t0
    //test if mstatus work
	csrr    a5,mstatus    
	li      t0, 0x800
	csrrs   zero, mie, t0
	csrr s8,mstatus
	andi t1, s8, 8
	bnez t1, putchar

returning:
	andi t5,t5,0
	andi t6,t6,0
	addi t5, t5, 96
	andi t4,t4,0
	addi t4,t4,2

begin:	
    li t3, PLIC_BASE_ADDR    #plic priority base
	add t3,t3, t6
	sw t4, 0(t3)
	addi t6, t6, 4
	bge  t5, t6, begin

	andi t4,t4,0
	addi t4,t4,0xff            # enable int 
	li t3, PLIC_ENABLE_BASE    #plic base address
	sb t4, 0(t3)
	sb t4, 1(t3)
	sb t4, 2(t3)
	sb t4, 3(t3)

	li t3, PLIC_THRESHOLD_ADDR #set threshold priority
	li t4, 0x1
	sw t4, 0(t3)
	ret


putchar:
	li s2, UART_BASE
uart_status:	
    lb s1, 12(s2)
	andi s1,s1,0x2
	bnez s1, uart_status
	andi s1,s1,0
	addi s1,s1,66
	sb s1, 4(s2)
	j returning


.p2align 2
vectored_trap_entry:
    #only plic implemented
    j trap_entry       #note: this is where all sychronous exceptions go (normal trap)
    nop
    j s_s_int
    nop
    j reserved
    nop
    j m_s_int
    nop
    j u_t_int
    nop
    j s_t_int
    nop
    j reserved
    nop
    j m_t_int
    nop
    j u_e_int
    nop
    j s_e_int
    nop
    j reserved
    nop
    j plic_interrupt_handler
    nop
    j reserved
    nop
    j reserved
    nop
    j reserved
    nop


plic_interrupt_handler:
    
    addi sp, sp, -2 * REGBYTES
    SREG ra, REGBYTES(sp)
    jal context_save

    #this following part is very similar to mach_plic_handler in
    #the driver for PLIC

    li t3, PLIC_CLAIM_ADDR
    lw t4, 0(t3)
    #code to print interrupt ID
    li s2, 0x00011300
    uarts:	
        lb s1, 12(s2)
        andi s1,s1,0x2
        bnez s1, uarts
        andi s1,s1,0
        add s1,s1,t4          // print int id
        sb s1, 4(s2)
	
    beqz t3, invalid_claim      #0 value returned if no interrupt
    li t4, 6
    ble t3, t4, PWM_interrupt   #corresponding IDs for PWM
    li t4, 22
    ble t3, t4, GPIO_interrupt  #corresponding IDs for GPIO
    li t4, 24
    ble t3, t4, I2C_interrupt   #corresponding IDs for GPIO
    li t4, 27
    ble t3, t4, UART_interrupt  #corresponding IDs for GPIO
    j invalid_claim

    invalid_claim:
        #nothing to do here, so we just finish plic_interrupt_handler
        #we ignore interrupt
        jal context_restore
        LREG ra, REGBYTES(sp)
        addi sp, sp, 2 * REGBYTES
        mret
    
    PWM_interrupt:
        jal PWM_ISR
        j exit_plic_handler
    
    GPIO_interrupt:
        jal GPIO_ISR
        j exit_plic_handler

    I2C_interrupt:
        jal I2C_ISR
        j exit_plic_handler
    
    UART_interrupt:
        jal UART_ISR
        j exit_plic_handler

    exit_plic_handler:
        #we write ID back into claim/complete register
        li t3, PLIC_CLAIM_ADDR
        lw t4, 0(t3)
        sw t4, 0(t3)

        #all tasks done
        jal context_restore
        LREG ra, REGBYTES(sp)
        addi sp, sp, 2 * REGBYTES
        #context restored
        mret

        

#the ISRs can now be written completely context free
#they do not have to worry about PLIC and associated bookkeeping
#for now, we do nothing with the interrupt
PWM_ISR:
    ret

GPIO_ISR:
    ret

I2C_ISR:
    ret

UART_ISR:
    ret

context_save:
    addi sp, sp, -31*REGBYTES
    #x0 is always 0
    #x1 = ra is already saved before this point
    SREG x2, 1*REGBYTES(sp)
    SREG x3, 2*REGBYTES(sp)
    SREG x4, 3*REGBYTES(sp)
    SREG x5, 4*REGBYTES(sp)
    SREG x6, 5*REGBYTES(sp)
    SREG x7, 6*REGBYTES(sp)
    SREG x8, 7*REGBYTES(sp)
    SREG x9, 8*REGBYTES(sp)
    SREG x10, 9*REGBYTES(sp)
    SREG x11, 10*REGBYTES(sp)
    SREG x12, 11*REGBYTES(sp)
    SREG x13, 12*REGBYTES(sp)
    SREG x14, 13*REGBYTES(sp)
    SREG x15, 14*REGBYTES(sp)
    SREG x16, 15*REGBYTES(sp)
    SREG x17, 16*REGBYTES(sp)
    SREG x18, 17*REGBYTES(sp)
    SREG x19, 18*REGBYTES(sp)
    SREG x20, 19*REGBYTES(sp)
    SREG x21, 20*REGBYTES(sp)
    SREG x22, 21*REGBYTES(sp)
    SREG x23, 22*REGBYTES(sp)
    SREG x24, 23*REGBYTES(sp)
    SREG x25, 24*REGBYTES(sp)
    SREG x26, 25*REGBYTES(sp)
    SREG x27, 26*REGBYTES(sp)
    SREG x28, 27*REGBYTES(sp)
    SREG x29, 28*REGBYTES(sp)
    SREG x30, 29*REGBYTES(sp)
    SREG x31, 30*REGBYTES(sp)
    ret

context_restore:
    LREG x2, 1*REGBYTES(sp)
    LREG x3, 2*REGBYTES(sp)
    LREG x4, 3*REGBYTES(sp)
    LREG x5, 4*REGBYTES(sp)
    LREG x6, 5*REGBYTES(sp)
    LREG x7, 6*REGBYTES(sp)
    LREG x8, 7*REGBYTES(sp)
    LREG x9, 8*REGBYTES(sp)
    LREG x10, 9*REGBYTES(sp)
    LREG x11, 10*REGBYTES(sp)
    LREG x12, 11*REGBYTES(sp)
    LREG x13, 12*REGBYTES(sp)
    LREG x14, 13*REGBYTES(sp)
    LREG x15, 14*REGBYTES(sp)
    LREG x16, 15*REGBYTES(sp)
    LREG x17, 16*REGBYTES(sp)
    LREG x18, 17*REGBYTES(sp)
    LREG x19, 18*REGBYTES(sp)
    LREG x20, 19*REGBYTES(sp)
    LREG x21, 20*REGBYTES(sp)
    LREG x22, 21*REGBYTES(sp)
    LREG x23, 22*REGBYTES(sp)
    LREG x24, 23*REGBYTES(sp)
    LREG x25, 24*REGBYTES(sp)
    LREG x26, 25*REGBYTES(sp)
    LREG x27, 26*REGBYTES(sp)
    LREG x28, 27*REGBYTES(sp)
    LREG x29, 28*REGBYTES(sp)
    LREG x30, 29*REGBYTES(sp)
    LREG x31, 30*REGBYTES(sp)
    addi sp, sp, 31 * REGBYTES
    #original ra saved on top of stack
    ret



#unimplemented interrupt handlers for the vectored interrupt are left here

s_s_int: mret
m_s_int: mret
reserved: mret
u_t_int: mret
s_t_int: mret
m_t_int: mret
u_e_int: mret
s_e_int: mret

.p2align 2
trap_entry: 
    addi sp, sp, -1 * REGBYTES
    SREG ra, 0(sp)
    jal context_save

    jal exception_handler

    jal context_restore
    LREG ra, 0(sp)
    addi sp, sp, 1 * REGBYTES
    mret

exception_handler:
	csrr t0,mcause
	la t1, _data1  #base address
	lw t2, 0(t1)  # offset value
	addi t2,t2,4 #t1 holds base address for storing trap occurences
	sw t2,0(t1)  
	add t1, t1,t2
	sw t0,0(t1)  
	ret

.p2align 0x2
.section .data 
_data1:
.word 	0
.word 	0
.word 	0
.word 	0
