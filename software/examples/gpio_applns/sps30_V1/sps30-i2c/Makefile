# See user_config.inc for build customizations
include user_config.inc
include default_config.inc
##changes made -2
TARGET = pinaka
BSP:=../../../bsp
bspinc:=$(BSP)/include
#bsp lib file path

bsplib:=$(BSP)/libs
#bsp board specific files path
bspboard:=$(BSP)/third_party/$(TARGET)/
bspdri:=$(BSP)/drivers
bsputils:=$(BSP)/utils
bspcore:=$(BSP)/core

.PHONY: all clean

#all: sps30_example_usage

#sps30_example_usage: clean
#	$(CC) $(CFLAGS) -o $@ ${sps30_i2c_sources} ${${CONFIG_I2C_TYPE}_sources} ${sps30_i2c_dir}/sps30_example_usage.c


#####

SHELL := /bin/bash # Use bash syntax
DC ?=
PROGRAM ?= sps30
#default target board
TARGET ?= artix7_35t
DEBUG ?=
MARCH ?= rv32imac
MABI  ?= ilp32
XLEN  ?= 32
FLAGS ?=
all: create_dir
	make $(PROGRAM).riscv

$(PROGRAM).riscv: create_dir 
	@riscv$(XLEN)-unknown-elf-gcc -march=$(MARCH) -mabi=$(MABI) -w $(DC) -mcmodel=medany -static -std=gnu99 -fno-builtin-printf $(FLAGS) -I$(bspinc) -I$(bspboard)  -c $@ ${sps30_i2c_sources} ${${CONFIG_I2C_TYPE}_sources} ${sps30_i2c_dir}/sps30_example_usage.c -o ./output/sps30.o -lm -lgcc
	@riscv$(XLEN)-unknown-elf-gcc -march=$(MARCH) -mabi=$(MABI) -T  $(bspboard)/link.ld ./output/sps30.o  $(GENLIB)/gen_lib/libshakti$(XLEN).a  -o ./output/$(PROGRAM).shakti -static -nostartfiles -lm -lgcc
	@riscv$(XLEN)-unknown-elf-objdump -D ./output/$(PROGRAM).shakti > ./output/$(PROGRAM).dump

create_dir:
	@mkdir -p ./output
#####
#newly add code 20/03/2020
crt.o: create_dir
	@riscv$(XLEN)-unknown-elf-gcc -march=$(MARCH) -mcmodel=medany -static -std=gnu99 -fno-common -fno-builtin-printf -D__ASSEMBLY__=1 -c $(bspinc)/crt.S -o ./output/crt.o

all: sps30_example_usage

sps30_example_usage: clean
	$(CC) $(CFLAGS) -o $@ ${sps30_i2c_sources} ${${CONFIG_I2C_TYPE}_sources} ${sps30_i2c_dir}/sps30_example_usage.c

clean:
	$(RM) sps30_example_usage

build:
	@riscv$(XLEN)-unknown-elf-ar rcs ./gen_lib/libshakti$(XLEN).a ./gen_lib/start.o ./gen_lib/trap.o ./gen_lib/util.o ./gen_lib/traps.o ./gen_lib/log.o ./gen_lib/printf.shakti ./gen_lib/uart.o ./gen_lib/i2c_driver.o ./gen_lib/clint_driver.o ./gen_lib/plic_driver.o ./gen_lib/init.o ./gen_lib/pwm_driver.o

