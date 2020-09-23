 #Created by Sathya Narayanan N
# Email id: sathya281@gmail.com

#   Copyright (C) 2019  IIT Madras. All rights reserved.

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>

SHELL := /bin/bash # Use bash syntax
XLEN=64
DC=

#default target board
TARGET ?= artix7_35t
DEBUG ?=
PROGRAM=fingerprint

ifeq ($(TARGET),artix7_35t)
	XLEN=32
	flags= -D ARTIX7_35T
	MARCH=rv32imac
	MABI=ilp32
endif

ifeq ($(TARGET),artix7_100t)
	XLEN=64
	flags= -D ARTIX7_100T
	MARCH=rv64imafdc
	MABI=lp64d
endif

ifeq ($(DEBUG),DEBUG)
	DC=-g
endif

BSP:=../../../bsp
#bsp incl file path
bspinc:=$(BSP)/include
#bsp lib file path
bsplib:=$(BSP)/libwrap
#bsp board specific files path
bspboard:=$(BSP)/third_party/$(TARGET)/
bspdri:=$(BSP)/drivers
bsputils:=$(BSP)/utils


all: create_dir
	make $(PROGRAM).riscv

upload: all
	@elf2hex 4 16384 ./output/$(PROGRAM).shakti 2147483648 > ./output/code.mem
	@echo -e "Converting Codemem to array"
	@gcc ../../../bsp/utils/elf_to_header.c -o ./output/elf_to_header
	@./output/elf_to_header
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf $(flags) -I$(bspinc) -I./output -c $(bsputils)/deploy.c -o ./output/deploy.o -march=$(MARCH) -lm -lgcc
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf  -I$(bspinc) -c $(bspboard)/uart.c -o ./output/uart.o -march=$(MARCH) -lm -lgcc
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf  -I$(bspinc) -c $(bspdri)/spi/spi_driver.c -o ./output/spi_driver.o -march=$(MARCH) -lm -lgcc
	@riscv$(XLEN)-unknown-elf-gcc -T $(bspboard)/link.ld ./output/deploy.o ./output/printf.shakti ./output/uart.o ./output/crt.o ./output/spi_driver.o -o ./output/deploy.shakti -static -nostartfiles
	@echo -e "Upload elf to Flash"
	@../../../bsp/utils/burnFlash.py ./output/deploy.shakti


$(PROGRAM).riscv: crt.o printf.shakti create_dir
	@echo -e "$(GREEN) Compiling code $(NC)"
	@echo $(TARGET)
	@echo -e "$(RED) Caveat: Code starts at 0x80000000. Configure RTL appropriately $(NC)"
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf $(flags) -I$(bspinc)  -I$(bspboard) -c ./$(PROGRAM).c -o ./output/$(PROGRAM).o -march=$(MARCH) -lm -lgcc
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf  -I$(bspinc)  -c $(bsplib)/util.c -o ./output/util.o -march=$(MARCH) -lm -lgcc		
	@riscv$(XLEN)-unknown-elf-gcc -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf  -I$(bspinc)  -c $(bspboard)/uart.c -o ./output/uart.o -march=$(MARCH) -lm -lgcc	
	@riscv$(XLEN)-unknown-elf-gcc -T $(bspboard)/link.ld  ./output/util.o ./output/$(PROGRAM).o ./output/uart.o ./output/printf.shakti ./output/crt.o -o ./output/$(PROGRAM).shakti -static -nostartfiles -lm -lgcc
	@riscv$(XLEN)-unknown-elf-objdump -D ./output/$(PROGRAM).shakti > ./output/$(PROGRAM).dump


create_dir:
	@mkdir -p ./output


crt.o: create_dir
	@riscv$(XLEN)-unknown-elf-gcc -march=$(MARCH) -mcmodel=medany -static -std=gnu99 -fno-common -fno-builtin-printf -D__ASSEMBLY__=1 -c $(bspinc)/crt.S -o ./output/crt.o

printf.shakti:
	@riscv$(XLEN)-unknown-elf-gcc -march=$(MARCH) -mcmodel=medany -static -std=gnu99 -fno-common -fno-builtin-printf  -c $(bsplib)/printf.c -o ./output/printf.shakti -I$(bspinc)
