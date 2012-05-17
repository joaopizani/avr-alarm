#########  AVR Project Makefile Template   #########
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
####################################################

## With heavy modifications to suit Arduino's oddities related to
## bootloader and programmer, and also including the option to generate
## and install a static library (.a archive) from this same Makefile.
##
## This makefile includes two targets for simulation using simulavr:
## The first target (simulate-gdb) will run your program with simulavr as
## an AVR-GDB server and connect AVR-GDB to it so that you can debug.
## The second target (simulate-vcd) allows you to run your program under
## simulavr and collect waveform traces in VCD format for later analysis.
##
## These modifications were done by
##
## João Paulo Pizani Flor <joaopizani@gmail.com>
## http://joaopizani.hopto.org


##### Target Specific Details
##### Customize these for your project

# Name of target controller 
# (e.g. 'at90s8515', see avr-gcc's mmcu options for possible values)
MCU=atmega168 # This is the MCU of Arduino Duemilanove

# target controller clock frequency in Hz
MCU_FREQ=16000000 # This is Arduino Duemilanove's freq

# id to use with programmer - default: PROGRAMMER_MCU=$(MCU)
PROGRAMMER_MCU=$(MCU)

# Name of our project - (use a single word, e.g. 'myproject')
PROJECTNAME=avr-alarm-test

# Set LIBRARY if the project is a static lib, leave undefined otherwise
LIBRARY=

# Source files. List C/C++/Assembly source files:
PRJSRC=../src/relative_queue.c \
	   ../src/alarm.c \
	   relative_queue_test.c \
	   alarm_test.c \
	   main.c

# Header files of the project. They will be installed in the case of a lib.
PRJHEADERS=../src/relative_queue.h \
		   ../src/alarm.h \
		   relative_queue_test.h \
		   alarm_test.h

# additional includes and libraries to link in are defined in paths.def
include paths.def
INC=${EXT_INCFLAGS}
LIBS=${EXT_LIBFLAGS}

# additional macro definition flags
EXT_DEFS=

# Optimization level - use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=1


#####  AVR Dude 'writeflash' options
AVRDUDE_PROGRAMMERID=arduino
AVRDUDE_PORT=/dev/ttyUSB0
AVRDUDE_BAUD=-b 19200  # 19200 baud for Arduino Duemilanove


##### Simulation options. Change only what you think is necessary
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
##### Now include non-variable definitions     #####
##### and the make targets themselves, which   #####
##### are in Makefile.targets                  #####
#####                                          #####
####################################################

include Makefile.targets

