/***************************************************************************
 * Project           			:  shakti devt board
 * Name of the file	     		:  boot.c
 * Created date			        :  3.10.2019
 * Brief Description of file            :  boot code to copy bare metal appln 
                                           from flash to physical memory.

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

#include "boot.h"

#define CODE_SIZE   131072
#define SPI_CR1     0x00020000
#define SPI_CR2     0x00020004
#define SPI_SR      0x00020008
#define SPI_DR1     0x0002000C
#define SPI_DR2     0x00020010
#define SPI_DR3     0x00020014
#define SPI_DR4	    0x00020018
#define SPI_DR5	    0x0002001C
#define SPI_CRCPR   0x00020020
#define SPI_RXCRCR  0x00020024
#define SPI_TXCRCR  0x00020028

// defining SPI_CR1 register

#define SPI_CPHA		(1 << 0)
#define SPI_CPOL		(1 << 1)
#define SPI_MSTR		(1 << 2)
#define SPI_BR(x)		(x << 3)
#define SPI_SPE  		(1 << 6)
#define SPI_LSBFIRST		(1 << 7)
#define SPI_SSI 		(1 << 8)
#define SPI_SSM			(1 << 9)
#define SPI_RXONLY		(1 << 10)
#define SPI_CRCL		(1 << 11)
#define SPI_CCRCNEXT		(1 << 12)
#define SPI_CRCEN		(1 << 13)
#define SPI_BIDIOE		(1 << 14)
#define SPI_BIDIMODE		(1 << 15)
#define SPI_TOTAL_BITS_TX(x)  	(x << 16)
#define SPI_TOTAL_BITS_RX(x)  	(x << 24)

// defining SPI_CR2 register

#define SPI_RX_IMM_START (1 << 16)
#define SPI_RX_START	 (1 << 15)
#define SPI_LDMA_TX      (1 << 14)
#define SPI_LDMA_RX 	 (1 << 13)
#define SPI_FRXTH	 (1 << 12)
#define SPI_DS(x)	 (x << 8)
#define SPI_TXEIE	 (1 << 7)
#define SPI_RXNEIE	 (1 << 6)
#define SPI_ERRIE	 (1 << 5)
#define SPI_FRF		 (1 << 4)
#define SPI_NSSP	 (1 << 3)
#define SPI_SSOE	 (1 << 2)
#define SPI_TXDMAEN	 (1 << 1)
#define SPI_RXDMAEN	 (1 << 0)

//defining SR register

#define SPI_FTLVL(x)  (x << 11)
#define SPI_FRLVL(x)  (x << 9)
#define SPI_FRE	      (1 << 8)
#define SPI_OVR	      (1 << 6)
#define SPI_MODF      (1 << 5)
#define SPI_CRCERR    (1 << 4)
#define TXE	      (1 << 1)
#define RXNE	      (1 << 0)

//pointers to register

int* spi_cr1 = (int*) SPI_CR1;
int* spi_cr2 = (int*) SPI_CR2;
int* spi_sr  = (int*) SPI_SR ;
int* spi_dr1 = (int*) SPI_DR1 ;
int* spi_dr2 = (int*) SPI_DR2 ;
int* spi_dr5 = (int*) SPI_DR5 ;

void set_spi(int* addr, int val)
{
	*addr = val;
}

int get_spi(int* addr)
{
	return *addr;
}

void spi_init(){
	set_spi(spi_cr1, (SPI_BR(7)|SPI_CPHA|SPI_CPOL));
}

void spi_tx_rx_start(){
	set_spi(spi_cr2, (SPI_RX_IMM_START));
}

void spi_enable(){
	set_spi(spi_cr1, (SPI_BR(7)|SPI_TOTAL_BITS_TX(4)|SPI_TOTAL_BITS_RX(16)|SPI_SPE));
}

void spi_rx_enable(){
	set_spi(spi_cr2, (SPI_RX_START));
}

int spi_rxne_enable(){
	int value = 0;
	int count = 0;

	while (!(value & 0x1))
	{
		waitfor(100);
		value = get_spi(spi_sr);
		if(count++ > 100)
			return -1;
	}

	return 1;
}

void waitfor(unsigned int secs) {

	unsigned int time = 0;

	while(time++ < secs);
}

int flash_cmd_to_read(int command, int addr)
{
	int dr5;

	set_spi(spi_dr1, command  |  (addr >> 8));
	set_spi(spi_dr2, addr << 24);
	set_spi(spi_dr5, 0);
	spi_tx_rx_start();
	set_spi(spi_cr1, (SPI_BR(7)|SPI_TOTAL_BITS_TX(48)|SPI_TOTAL_BITS_RX(32)|SPI_SPE|SPI_CPHA|SPI_CPOL));
	waitfor(20);

	if(spi_rxne_enable()) {
		dr5 = *spi_dr5;
	}
	return dr5;
}

int flash_read(int address)
{
	int read_value = flash_cmd_to_read(0x0c000000,address);
	return read_value;
}

int flash_cmd_read(int command)
{
	int dr1, dr2, dr5;
	set_spi(spi_dr1, command);
	set_spi(spi_dr5, command);
	spi_tx_rx_start();
	set_spi(spi_cr1, (SPI_BR(7)|SPI_TOTAL_BITS_TX(8)|SPI_TOTAL_BITS_RX(32)|SPI_SPE|SPI_CPHA|SPI_CPOL));

	if(spi_rxne_enable()) {
		dr5 = *spi_dr5;
		dr2 = *spi_dr2;
	}
	return dr5;
}

int flash_status_register_read()
{
	int stat = 0x3;

	while (stat & 0x03){
		stat = flash_cmd_read(0x05000000);
	}

	return 0;
}

int flash_device_id()
{
	int dr1, dr2, dr3;
	int val1, val2;

	set_spi(spi_dr1, 0x9f000000);
	set_spi(spi_dr5, 0x9f000000);
	spi_tx_rx_start();
	set_spi(spi_cr1, (SPI_BR(7)|SPI_TOTAL_BITS_TX(8)|SPI_TOTAL_BITS_RX(24)|SPI_SPE|SPI_CPHA|SPI_CPOL));

	if(1 == spi_rxne_enable()) {
		dr3 = *spi_dr5;
		dr2 = *spi_dr2;
	}
	else
	{
		return -1;
	}

	val1 = ( dr3 >> 16 ) & 0xff; //bitExtracted(dr3, 8, 17);
	val2 = dr3 & 0xffff; //bitExtracted(dr3, 16, 1);

 	printf("Device ID %x \n", val1);
 //	printf("extracted device id %x \n",val2); 

	return 1;	
}

void jump_2_ram()
{
//	printf("\n%s\n","Control transferred to RAM");
	asm volatile("fence.i");
	asm volatile( "li x30, 0x80000000" "\n\t"
			"jr x30" "\n\t"
			:
			:
			:"x30","cc","memory"
		    );
}

void main()
{
	unsigned int read_address;  // read/write from/to this address
	int* bram_address = (int*) 0x80000000;
	int read_value, i;
	int size;
        char isa[40];
	int retval = 0;
	int arch = 0;
	int hart = 0;

	isa[i++] = 'R';
	isa[i++] = 'V';
	isa[23] = '\0';
	isa[24] = '\0';
	isa[30] = '\0';

	if(__riscv_xlen == 64 )
	{
		isa[i++] = '6';
		isa[i++] = '4';
	}else
	{
		isa[i++] = '3';
		isa[i++] = '2';
	}

	asm volatile(
	     "csrr %[arch], marchid\n"
	     :
	     [arch]
	     "=r"
	     (arch)
	    );

	asm volatile(
	     "csrr %[hart], mhartid\n"
	     :
	     [hart]
	     "=r"
	     (hart)
	    );

	asm volatile(
	     "csrr %[retval], misa\n"
	     :
	     [retval]
	     "=r"
	     (retval)
	    );

	if (retval & 0x1)
		isa[i++]='A';

	if (retval & 0x2)
		isa[i++]='B';

	if (retval & 0x4)
		isa[i++]='C';

	if (retval & 0x8)
		isa[i++]='D';

	if (retval & 0x10)
		isa[i++]='e';

	if (retval & 0x20)
		isa[i++]='F';

	if (retval & 0x40)
		isa[i++]='g';

	if (retval & 0x80)
		isa[i++]='h';

	if (retval & 0x100)
		isa[i++]='I';

	if (retval & 0x200)
		isa[i++]='j';

/*
	if (retval & 400)
		isa[i++]='k';

	if (retval &800)
		isa[i++]='l';
*/

	if (retval & 0x1000)
		isa[i++]='M';

	if (retval & 0x2000)
		isa[i++]='N';

/*
	if (retval & 4000)
		isa[i++]='o';

	if (retval & 8000)
		isa[i++]='p';
*/

	if (retval & 0x10000)
		isa[i++]='q';

/*
	if (retval & 20000)
		isa[i++]='r';
*/

	if (retval & 0x40000)
		isa[i++]='S';

	/*
	   if (retval & 80000)
	   isa[i++]='t';
	 */

	if (retval & 0x100000)
		isa[i++]='U';
	isa[i]='\0';

	printf("%s",bootlogo);
	printf("Booting on Pinaka, hart %d\n",hart);
	printf("Welcome to the world of Shakti !\n");
	printf("Pinaka is a SoC build on top of Artix 7 35 T.\n");
	printf("The core belongs to Shakti E class, 32 bit.\n");
	printf("Supported ISA: %s.\n", isa);
	printf("Processor Arch ID: %d.\n", arch);
	printf("%s\n","Booting... vspi1.0\n");

	spi_init();

	if(1 == flash_device_id() )
	{
		read_address = 0x00b00000;

		size = flash_read(read_address);

		read_address=read_address+4;

		if(size == 0 || size == 0xFFFFFFFF)
		{
			printf("No content to boot \n");
			asm volatile ("ebreak");
		}

		for(i = 0; i < size;i++ )
		{
			read_value= flash_read(read_address);

			*(bram_address) = read_value;

			//printf("read address %x data:  %x      write address: %x data:  %x \n", read_address,read_value, bram_address, *(bram_address));

			read_address = read_address+4;
			bram_address++;

			if(i%1024 == 0)
			{
				printf(".");
			       //printf("read address %x data:  %x      write address: %x data:  %x :: %d\n", read_address,read_value, bram_address, *(bram_address),i);
			}
		}
	}
	else
	{
		printf("Wrong device id \n");

		asm volatile ("ebreak");
	}

	jump_2_ram();

	asm volatile ("ebreak");
}
