####################################################
#####                                          #####
##### You shouldn't need to edit anything      #####
##### below to use the makefile but may wish   #####
##### to override a few of the flags           #####
##### nonetheless                              #####
#####                                          #####
####################################################

##### Macro definitions, CPU frequency and simulation virtual I/O
DEFS=-DF_CPU=$(MCU_FREQ) \
	 -DSIM_WRITE="(*((volatile char *) $(SIM_WRITE_ADDR)))" \
	 -DSIM_READ="(*((volatile char *) $(SIM_READ_ADDR)))" \
	 -DSIM_EXIT="(*((volatile char *) $(SIM_EXIT_ADDR)))"


##### Simulation definitions
ifndef SIM_DIR
SIM_DIR=sim
endif

SIM_STDOUT_FILENAME=$(PROJECTNAME)-sim-stdout.txt
SIM_STDERR_FILENAME=$(PROJECTNAME)-sim-stderr.txt
SIM_WAVES_FILENAME=$(PROJECTNAME)-wav.vcd
SIM_TRACES_FILENAME=$(PROJECTNAME)-traces

ifndef SIM_TRACES
SIM_TRACES=$(SIM_DIR)/$(SIM_TRACES_FILENAME)
endif

ifndef SIM_TIME
SIM_TIME=200
endif

SIM_STDOUT=$(SIM_DIR)/$(SIM_STDOUT_FILENAME)
SIM_STDERR=$(SIM_DIR)/$(SIM_STDERR_FILENAME)
SIM_WAVES=$(SIM_DIR)/$(SIM_WAVES_FILENAME)


##### Executables
CC=avr-gcc
AR=avr-ar
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size
AVRDUDE=avrdude
REMOVE=rm -f


##### Compiler, linker, assembler, archiver flags
# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler flags
CFLAGS=-I. $(INC) -g -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums -funsigned-bitfields -funsigned-char \
	-Wall -Wstrict-prototypes -Wa,-ahlms=$(firstword $(filter %.lst, $(<:.c=.lst))) \
    $(DEFS)

# c++ specific flags
CPPFLAGS=-fno-exceptions \
	-Wa,-ahlms=$(firstword \
	$(filter %.lst, $(<:.cpp=.lst)) \
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler flags
ASMFLAGS =-I. $(INC) -mmcu=$(MCU) -x assembler-with-cpp \
    -Wa,-gstabs,-ahlms=$(firstword $(<:.S=.lst) $(<.s=.lst))

# linker flags
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU) -lm $(LIBS)

# static library archiver (ar) flags
ARFLAGS=cqs



##### Automatic target names
DUMPTRG=$(PROJECTNAME).s
HEXROMTRG=$(PROJECTNAME).hex 
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE=gdbinit-$(PROJECTNAME)
INSTALL_PREFIX=$(PREFIX)
INCLUDE_PREFIX=$(INSTALL_PREFIX)/include/$(PROJECTNAME)
LIB_PREFIX=$(INSTALL_PREFIX)/lib/$(PROJECTNAME)

EXETRG=$(PROJECTNAME).out
LIBTRG=lib$(PROJECTNAME).a
ifdef LIBRARY
	TRG=$(LIBTRG)
else
	TRG=$(EXETRG)
endif



##### Define all object files.
# Start by splitting source files by type
CPPFILES=$(filter %.cpp, $(PRJSRC))
CCFILES=$(filter %.cc, $(PRJSRC))
BIGCFILES=$(filter %.C, $(PRJSRC))
CFILES=$(filter %.c, $(PRJSRC))
ASMFILES=$(filter %.S, $(PRJSRC))

# List all object files we need to create
OBJDEPS=$(CFILES:.c=.o) $(CPPFILES:.cpp=.o) $(BIGCFILES:.C=.o) $(CCFILES:.cc=.o) $(ASMFILES:.S=.o)

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s)) 



##### Enough definitions, now the make stuff
.SUFFIXES : .c .cc .cpp .C .o .out .s .S .hex .ee.hex .h .hh .hpp
.PHONY: writeflash install clean stats gdbinit

# Make targets:
# all, disasm, stats, hex, writeflash, install, clean, simulate-{gdb,vcd}
all: $(TRG)

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXTRG)

writeflash: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID) -p $(PROGRAMMER_MCU) \
    -P $(AVRDUDE_PORT) -e  $(AVRDUDE_BAUD) -U flash:w:$(HEXROMTRG)

install: $(LIBTRG)
	install -d $(LIB_PREFIX)
	install $(LIBTRG) $(LIB_PREFIX)
ifdef PRJHEADERS
	install -d $(INCLUDE_PREFIX)
	install $(PRJHEADERS) $(INCLUDE_PREFIX)
endif

###### Generating the targets
$(DUMPTRG): $(TRG)
	$(OBJDUMP) -S  $< > $@

$(EXETRG): $(OBJDEPS)
	$(CC) -o $(TRG) $(OBJDEPS) $(LDFLAGS)

$(LIBTRG): $(OBJDEPS)
	$(REMOVE) $@
	$(AR) $(ARFLAGS) $@ $(OBJDEPS)

###### Generating intermediary files
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@

# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@

# object from C
.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

# object from C++ (.cc, .cpp, .C files)
.cc.o .cpp.o .C.o :
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@

###### Generating hex files
.out.hex:
	$(OBJCOPY) -j .text -j .data -O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O $(HEXFORMAT) $< $@


######  Generating a gdb initialisation file
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


###### Simulation targets, using simulavr and (possibly) avr-gdb
simulate-gdb: $(GDBINITFILE)
	mkdir -p $(SIM_DIR)
	simulavr -d $(SIM_MCU) -F $(MCU_FREQ) -f $(EXETRG) \
    -W $(SIM_WRITE_ADDR),- -R $(SIM_READ_ADDR),- -e $(SIM_EXIT_ADDR) \
	-g 2> $(SIM_STDERR) 1> $(SIM_STDOUT) &
	@sleep 1
	avr-gdbtui -x $(GDBINITFILE)

simulate-vcd: $(EXETRG)
	mkdir -p $(SIM_DIR)
	simulavr -d $(SIM_MCU) -F $(MCU_FREQ) -f $(EXETRG) \
    -W $(SIM_WRITE_ADDR),- -R $(SIM_READ_ADDR),- -e $(SIM_EXIT_ADDR) \
	-c vcd:$(SIM_TRACES):$(SIM_WAVES) -m $(SIM_TIME)000000



##### Cleanup
clean:
	$(REMOVE) $(LIBTRG) $(LIBTRG).map
	$(REMOVE) $(EXETRG) $(EXETRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)
	$(REMOVE) $(SIM_STDOUT) $(SIM_STDERR) $(SIM_WAVES)
	$(REMOVE) *~

