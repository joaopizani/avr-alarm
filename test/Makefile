
#########  AVR Project Makefile Template   #########
######                                        ######
######    Copyright (C) 2003-2005,Pat Deegan, ######
######            Psychogenic Inc             ######
######          All Rights Reserved           ######
######                                        ######
###### You are free to use this code as part  ######
###### of your own applications provided      ######
###### you keep this copyright notice intact  ######
###### and acknowledge its authorship with    ######
###### the words:                             ######
######                                        ######
###### "Contains software by Pat Deegan of    ######
###### Psychogenic Inc (www.psychogenic.com)" ######
######                                        ######
###### If you use it as part of a web site    ######
###### please include a link to our site,     ######
###### http://electrons.psychogenic.com  or   ######
###### http://www.psychogenic.com             ######
######                                        ######
####################################################


## With some modifications to suit Arduino's oddities related to
## bootloader and programmer, and also including the option to generate
## and install a static library (.a archive) from this same Makefile
##
## These modifications were done by
##
## João Paulo Pizani Flor <joaopizani@gmail.com>
## http://joaopizani.hopto.org


#####         Target Specific Details          #####
#####     Customize these for your project     #####

# Name of target controller 
# (e.g. 'at90s8515', see the available avr-gcc's options for possible values)
MCU=atmega168p

# target controller clock frequency in Hz
MCU_FREQ=16000000

# id to use with programmer
# default: PROGRAMMER_MCU=$(MCU)
PROGRAMMER_MCU=atmega168

# Name of our project - (use a single word, e.g. 'myproject')
PROJECTNAME=avr-alarm-test

# Set LIBRARY if the project shall compile to a static lib, leave undefined otherwise
LIBRARY=

# Source files
# List C/C++/Assembly source files:
PRJSRC=relative_queue_test.c \
	   alarm_test.c \
	   main.c


# Header files
# List all headers of the project. They will be installed in the case of a lib.
PRJHEADERS=relative_queue_test.h \
		   alarm_test.h


# additional includes and libraries to link in are defined in paths.def
include paths.def
INC=${EXT_INCFLAGS}
LIBS=${EXT_LIBFLAGS}


# additional macro definition flags
EXT_DEFS=

# Optimization level - use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=1


#####      AVR Dude 'writeflash' options       #####
AVRDUDE_PROGRAMMERID=arduino
AVRDUDE_PORT=/dev/ttyUSB0

# Necessary for arduino to set this up as 19200
AVRDUDE_BAUD=-b 19200


#### Simulation options. Change only what you think is necessary
SIM_MCU=$(PROGRAMMER_MCU)
SIM_EXIT_ADDR=0xDE
SIM_WRITE_ADDR=0xFF
SIM_READ_ADDR=0xFE

#### Leave these undefined if you want to accept the defaults. Defaults are:
#### SIM_TIME=200 (ms),  SIM_DIR=sim,  SIM_TRACES=$(SIM_DIR)/traces-selected
SIM_TIME=
SIM_DIR=
SIM_TRACES=



####################################################
#####                Config Done               #####
#####                                          #####
##### You shouldn't need to edit anything      #####
##### below to use the makefile but may wish   #####
##### to override a few of the flags           #####
##### nonetheless                              #####
#####                                          #####
####################################################


#### Macro definitions
DEFS=-DF_CPU=$(MCU_FREQ) \
	 -DSIM_WRITE="(*((volatile char *) $(SIM_WRITE_ADDR)))" \
	 -DSIM_READ="(*((volatile char *) $(SIM_READ_ADDR)))" \
	 -DSIM_EXIT="(*((volatile char *) $(SIM_EXIT_ADDR)))"

#### Simulation
ifndef SIM_DIR
SIM_DIR=sim
endif

SIM_STDOUT_FILENAME=simulation-stdout.txt
SIM_STDERR_FILENAME=simulation-stderr.txt
SIM_WAVES_FILENAME=waves.vcd
SIM_TRACES_FILENAME=traces-selected

ifndef SIM_TRACES
SIM_TRACES=$(SIM_DIR)/$(SIM_TRACES_FILENAME)
endif

ifndef SIM_TIME
SIM_TIME=200
endif

SIM_STDOUT=$(SIM_DIR)/$(SIM_STDOUT_FILENAME)
SIM_STDERR=$(SIM_DIR)/$(SIM_STDERR_FILENAME)
SIM_WAVES=$(SIM_DIR)/$(SIM_WAVES_FILENAME)


##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
    $(DEFS)                                 \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

# c++ specific flags
CPPFLAGS=-fno-exceptions               \
	-Wa,-ahlms=$(firstword         \
	$(filter %.lst, $(<:.cpp=.lst))\
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler
ASMFLAGS =-I. $(INC) -mmcu=$(MCU)        \
	-x assembler-with-cpp            \
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst) $(<.s=.lst))

# linker
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU) \
	-lm $(LIBS)

# static library archiver (ar) flags
ARFLAGS=cqs



##### executables ####
CC=avr-gcc
AR=avr-ar
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size
AVRDUDE=avrdude
REMOVE=rm -f

##### automatic target names ####
EXETRG=$(PROJECTNAME).out
LIBTRG=lib$(PROJECTNAME).a
ifdef LIBRARY
	TRG=$(LIBTRG)
else
	TRG=$(EXETRG)
endif

DUMPTRG=$(PROJECTNAME).s
HEXROMTRG=$(PROJECTNAME).hex 
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE=gdbinit-$(PROJECTNAME)
INSTALL_PREFIX=$(PREFIX)
INCLUDE_PREFIX=$(INSTALL_PREFIX)/include/$(PROJECTNAME)
LIB_PREFIX=$(INSTALL_PREFIX)/lib/$(PROJECTNAME)


# Define all object files.

# Start by splitting source files by type
#  C++
CPPFILES=$(filter %.cpp, $(PRJSRC))
CCFILES=$(filter %.cc, $(PRJSRC))
BIGCFILES=$(filter %.C, $(PRJSRC))
#  C
CFILES=$(filter %.c, $(PRJSRC))
#  Assembly
ASMFILES=$(filter %.S, $(PRJSRC))


# List all object files we need to create
OBJDEPS=$(CFILES:.c=.o)    \
	$(CPPFILES:.cpp=.o)\
	$(BIGCFILES:.C=.o) \
	$(CCFILES:.cc=.o)  \
	$(ASMFILES:.S=.o)

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly 
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s)) 


.SUFFIXES : .c .cc .cpp .C .o .out .s .S \
	.hex .ee.hex .h .hh .hpp


.PHONY: writeflash install clean stats gdbinit


# Make targets:
# all, disasm, stats, hex, writeflash, install, clean
all: $(TRG)

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXTRG)

writeflash: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(PROGRAMMER_MCU) -P $(AVRDUDE_PORT) -e        \
	 $(AVRDUDE_BAUD) -U flash:w:$(HEXROMTRG)

install: $(LIBTRG)
	install -d $(LIB_PREFIX)
	install $(LIBTRG) $(LIB_PREFIX)
ifdef PRJHEADERS
	install -d $(INCLUDE_PREFIX)
	install $(PRJHEADERS) $(INCLUDE_PREFIX)
endif


# How to generate the targets
$(DUMPTRG): $(TRG)
	$(OBJDUMP) -S  $< > $@


$(EXETRG): $(OBJDEPS)
	$(CC)  -o $(TRG) $(OBJDEPS) $(LDFLAGS)

$(LIBTRG): $(OBJDEPS)
	$(REMOVE) $@
	$(AR) $(ARFLAGS) $@ $(OBJDEPS)



#### Generating assembly ####
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@


#### Generating object files ####
# object from C
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@


# object from C++ (.cc, .cpp, .C files)
.cc.o .cpp.o .C.o :
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
.out.hex:
	$(OBJCOPY) -j .text                    \
		-j .data                       \
		-O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom                  \
		--change-section-lma .eeprom=0 \
		-O $(HEXFORMAT) $< $@




#####  Generating a gdb initialisation file    #####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject           #####
gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(EXETRG)
	@echo "file $(EXETRG)" > $(GDBINITFILE)
	
	@echo "target remote localhost:1212" \
		                >> $(GDBINITFILE)
	
	@echo "load"        >> $(GDBINITFILE) 
	@echo "break main"  >> $(GDBINITFILE)
	@echo "continue"    >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"



#### Simulation targets, using simulavr and (possibly) avr-gdb ####
simulate-gdb: $(GDBINITFILE)
	mkdir -p $(SIM_DIR)
	simulavr -d $(SIM_MCU) -F $(MCU_FREQ) -f $(EXETRG) -W $(SIM_WRITE_ADDR),- -R $(SIM_READ_ADDR),- -e $(SIM_EXIT_ADDR) \
	-g 2> $(SIM_STDERR) 1> $(SIM_STDOUT) &
	@sleep 1
	avr-gdbtui -x $(GDBINITFILE)

simulate-vcd: $(EXETRG)
	mkdir -p $(SIM_DIR)
	simulavr -d $(SIM_MCU) -F $(MCU_FREQ) -f $(EXETRG) -W $(SIM_WRITE_ADDR),- -R $(SIM_READ_ADDR),- -e $(SIM_EXIT_ADDR) \
	-c vcd:$(SIM_TRACES):$(SIM_WAVES) -m $(SIM_TIME)000000



#### Cleanup ####
clean:
	$(REMOVE) $(LIBTRG) $(LIBTRG).map
	$(REMOVE) $(EXETRG) $(EXETRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)
	$(REMOVE) $(SIM_STDOUT) $(SIM_STDERR) $(SIM_WAVES)
	$(REMOVE) *~
	


#####                    EOF                   #####

