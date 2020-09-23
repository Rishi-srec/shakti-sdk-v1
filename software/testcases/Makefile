#Created by Sathya Narayanan N & Abhinav Ramnath
# Email id: sathya281@gmail.com & abhinavramnath13@gmail.com

#   Copyright (C) 2019  IIT Madras. All rights reserved.

#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.

#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

SHELL := /bin/bash # Use bash syntax

export XLEN
export MABI
export MARCH
export FLAGS
export DC
export filepath

# appln and target board
PROGRAM ?=all
TARGET ?=artix7_35t
DEBUG ?=
UPLOAD ?=
FLASH ?=
XLEN ?=32
filepath ?= 
D ?= 

GENLIB:=$(shell pwd)/../../software/examples/
# Invoke all possible combinations of applns for different targets
# There is a provision to make in the exact folder where code is written
# We are using that provision to invole appropriate makefile scripts from top folder
all: check do finish
	@echo "TARGET is" $(TARGET)
check:
ifeq ($(DEBUG),DEBUG)
	@$(eval D = -g)
endif
ifeq ($(TARGET),artix7_35t)
	@$(eval XLEN = 32)
	@$(eval FLAGS= -D ARTIX7_35T)
	@$(eval MARCH=rv32imac)
	@$(eval MABI=ilp32)
	@$(eval INTERFACE=$(bspboard)/ftdi)
	@$(eval DC =$(FLAGS) $(D) )
else
ifeq ($(TARGET),artix7_100t)
	@$(eval XLEN = 64)
	@$(eval FLAGS= -D ARTIX7_100T)
	@$(eval MARCH=rv64imac)
	@$(eval MABI=lp64)
	@$(eval INTERFACE=$(bspboard)/ftdi)
	@$(eval DC =$(FLAGS) $(D) )
else
ifeq ($(TARGET),aardonyx)
	@echo $(filepath)
	@$(eval XLEN = 32)
	@$(eval FLAGS= -D AARDONYX)
	@$(eval MARCH=rv32imac)
	@$(eval MABI=ilp32)
	@$(eval UPLOADER=$(UPLOADDIR)/micron)
	@$(eval FLASHSPEC=$(bspdri)/qspi/qspi_micron.c)
	@$(eval INTERFACE=$(bspboard)/jlink)
	@$(eval DC =$(FLAGS) $(D) )
else
ifeq ($(TARGET),pinaka)                         # e-class 35t rv32imac
	@$(eval XLEN = 32)
	@$(eval FLAGS= -D pinaka)
	@$(eval MARCH=rv32imac)
	@$(eval MABI=ilp32)
	@$(eval INTERFACE=$(bspboard)/ftdi)
	@$(eval DC =$(FLAGS) $(D))
else
ifeq ($(TARGET),parashu)                         # e-class 100t rv32imac
	@$(eval XLEN = 32)
	@$(eval FLAGS= -D parashu)
	@$(eval MARCH=rv32imac)
	@$(eval MABI=ilp32)
	@$(eval INTERFACE=$(bspboard)/ftdi)
	@$(eval DC =$(FLAGS) $(D))
else
ifeq ($(TARGET),vajra)                         # c-class 100t rv64imacsu
	@$(eval XLEN = 64)
	@$(eval FLAGS= -D vajra)
	@$(eval MARCH=rv64imac)
	@$(eval MABI=lp64)
	@$(eval INTERFACE=$(bspboard)/ftdi)
	@$(eval DC =$(FLAGS) $(D))
else
	@echo -e "$(TARGET) not supported"
endif
endif
endif
endif
endif
endif

clean:
	@rm -rf ./$(filepath)/output/*.txt
	@rm -rf ./$(filepath)/output/*.bin
	@find -type d -name output -a -prune -exec rm -rf {} \;


do:	
ifeq ($(PROGRAM),all)
	@echo -e "make all testcases"
	cd ./uart && $(MAKE) all TARGET=$(TARGET)
	cd ./csr && $(MAKE) all TARGET=$(TARGET)
endif

finish:
ifeq ($(PROGRAM),all)
	@echo -e "make all over"
else
ifeq ($(CLEAR),)
	cd $(filepath) && $(MAKE) $(PROGRAM).riscv TARGET=$(TARGET) DEBUG=$(DEBUG)
else
ifeq ($(PROGRAM),)
	cd $(filepath)/../ && $(MAKE) clean CLEAR=$(CLEAR)
else
	cd $(filepath)/../ && $(MAKE) clean PROGRAM=$(PROGRAM) CLEAR=$(CLEAR)
endif
endif
endif
