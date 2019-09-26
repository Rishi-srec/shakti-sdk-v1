


int getchar()
{
 register char a0 asm("a0");
 register int a1 asm("a1") = 0;
       asm volatile ("li t1, 0x11300" "\n\t" //The base address of UART config registers
           		  	"uart_statusr: lb t2, 12(t1)" "\n\t"
    				"andi t2, t2, 0x8" "\n\t"
	    			"beqz t2, uart_statusr" "\n\t"
                    "lb a0, 8(t1)"  "\n\t"      //The base address of UART data register
                    :
                    :
                    :"a0","t1","t2","cc","memory");


   return a0;
}

int main()
{

int a,b,c;
printf("Enter 2 numbers\n");			//Getting the inputs
a=getchar() - '0';
printf(" a =  %d",a);
b=getchar() - '0';
printf(" b =  %d",b);
printf("Enter operation 1.Add 2.Subtract 3.Multiply 4.Divide\n");
c=getchar() - '0';
printf(" c =  %d",c);
register int a1 asm ("x18");
register int a2 asm ("x19");
register int a3 asm ("x20");
a1=a;
a2=b;
a3=c;
register int a4 asm ("x21");			//register to store output of computation
asm volatile(
	"li x22,1""\n\t"
	"li x23,2""\n\t"
	"li x24,3""\n\t"			//using 4 registers to compare with the options
	"li x25,4""\n\t"
);

asm volatile(
	"beq x20,x22,addlabel""\n\t"
	"beq x20,x23,sublabel""\n\t"
	"beq x20,x24,mullabel""\n\t"
	"beq x20,x25,divlabel""\n\t"		//conditional branching based on choice
	"addlabel:""\n\t"
	"add x21,x18,x19""\n\t"	
	"j end""\n\t"
	"sublabel:""\n\t"
	"sub x21,x18,x19""\n\t"
	"j end""\n\t"
	"mullabel:""\n\t"
	"mul x21,x18,x19""\n\t"
	"j end""\n\t"	
	"divlabel:""\n\t"
	"div x21,x18,x19""\n\t"
	"j end""\n\t"
);

asm volatile(
	"end:""\n\t"				//end of branch
);
printf("\nThe result is %d",a4);
}


