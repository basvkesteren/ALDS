##
#    ALDS (ARM LPC Driver Set)
#
#    Makefile:
#             Makefile ALDS
#
#    copyright:
#              Copyright (c) 2006-2008 Bastiaan van Kesteren <bastiaanvankesteren@gmail.com>
#
#              This program comes with ABSOLUTELY NO WARRANTY; for details see the file LICENSE.
#              This program is free software; you can redistribute it and/or modify it under the terms
#              of the GNU General Public License as published by the Free Software Foundation; either
#              version 2 of the License, or (at your option) any later version.
#
#    remarks:
#            -This file describes the compilation-process of the code.
#

NAME            = ALDS

##############
# Settings
##

# The MCU in use. Valid options are LPC2101,LPC2102,LPC2103,LPC2104,LPC2105,LPC2106,LPC2141,LPC2142,LPC2144,LPC2146 and LPC2148
MCU              = LPC2103

# Oscillator frequency in kHz
FOSC             = 12000

# Optimization
 # Optimization-setting of the compiler. Chose -O0 when testing, -O1 or -O2
 # when everything seems to work OK. Other possible settings are -Os (when
 # size is important) or -O3 (when you feel like breaking stuff...)
OPT              = -Os
 # It's possible to remove unused code (functions, mostly) from the binary.
 # Use REMOVEUNUSED = TRUE to enable this, or FALSE to disable it.
 # Note that enabling this might cause problem with some interrupt-related
 # functions, or stuff written in assembler. When testing, it's better do disable this.
 # NOTE: the only real problem i've found with this is that, when enabled, debugging
 # asm-code is problematic. There appears to be a long-standing bug in GCC where
 # debugging-info of the removed code is not removed, so GDB gets confused. For the
 # GCC version i'm using this apparently shows up when debugging asm code
REMOVEUNUSED     = TRUE

# Are we running from ROM or RAM?
 # Chose RUN_FROM_ROM (rom/flash) or RUN_FROM_RAM (ram)
MEM              = RUN_FROM_ROM

# Thumb mode
 # Make use of the thumb instruction set where possible. Thumb is smaller, but slower.
USETHUMB         = FALSE

# Generate list files?
 # If set to TRUE, a .lst file is generated for each sourcefile.
GENERATELISTS    = TRUE

# Debug settings
 # 'normal' debug is enabled with -g.
 # leave this empty to disable all debug-info. For other options, see the GCC manpage
 # The '-mpoke-function-name' is needed for functionname lookup (see drivers/backtrace.c)
 # NOTE: the '-gstabs+' parameter (which enables extended stabs debugging symbols) causes
 #       trouble when thumb-mode is enabled
DBG              = -g3 -ggdb -mpoke-function-name

 # Interface to use for OpenOCD JTAG debugging
 # Known types are: 0 -> Wiggler and clones
 #                  1 -> Olimex ARM-USB-OCD
JTAGINTERFACE    = 1

# Libraries
 # Add libraries to link into the resulting binary here; each library is preceded by '-l'
 # Most commonly used library would be the math library, which is called 'm'. So to link the math library, use '-lm'
LIBRARIES        = -lm

# When set to a value > 0, GCC's Wstack-usage argument is used
STACKWARNING	 = 256

##############
# what to compile
##

 # Following file is included by default, and (should) contain project specific stuff
include Makefile_project.inc

# Add all driver-code
 # We just add all C/assembler code in the 'drivers' dir.
drivers         = $(wildcard drivers/*.c) $(wildcard drivers/*.S)
 # And the C/assembler code in the 'board' dir, too
drivers        += $(wildcard drivers/board/*.c) $(wildcard drivers/board/*.S)

# path-settings
 # The '@' here will tell make that we don't want to see the actual commands, which makes the build-output
 # a bit easier to read.
PREFIX          = @
 # Where is the compiler located?
PATHPREFIX      = arm-none-eabi-

AS              = $(PREFIX)$(PATHPREFIX)as
CC              = $(PREFIX)$(PATHPREFIX)gcc
CPP             = $(PREFIX)$(PATHPREFIX)g++
SIZE            = $(PREFIX)$(PATHPREFIX)size
OBJCOPY         = $(PREFIX)$(PATHPREFIX)objcopy
READELF         = $(PREFIX)$(PATHPREFIX)readelf
GDB             = $(PREFIX)$(PATHPREFIX)gdb
INSIGHT         = $(PATHPREFIX)insight
OPENOCD         = openocd
DOXYGEN         = $(PREFIX)doxygen
SPARSE          = $(PREFIX)sparse

SHELL           = bash

##############
# Following code is not meant to be changed
##

SRC             = main.c callback.c $(filter-out $(wildcard drivers/*_ARM.c) $(wildcard drivers/*_ARM.S), $(drivers)) $(project)
SRC_ARM         = crt0.S $(filter $(wildcard drivers/*_ARM.c) $(wildcard drivers/*_ARM.S), $(drivers)) $(project_arm)

OBJ             = $(patsubst %.c,%.o,$(filter %.c,$(SRC)))
ASMOBJ          = $(patsubst %.S,%.o,$(filter %.S,$(SRC)))

OBJ_ARM         = $(patsubst %.c,%.o,$(filter %.c,$(SRC_ARM)))
ASMOBJ_ARM      = $(patsubst %.S,%.o,$(filter %.S,$(SRC_ARM)))

SU              = $(patsubst %.c,%.su,$(filter %.c,$(SRC)))

# output settings
ELF             = $(NAME).elf
HEX             = $(NAME).hex
BIN             = $(NAME).bin
MAP             = $(NAME).map
LNK             = ./Linkerscript
LST             = $(patsubst %.o,%.lst,$(OBJ)) $(patsubst %.o,%.lst,$(ASMOBJ)) $(patsubst %.o,%.lst,$(OBJ_ARM)) $(patsubst %.o,%.lst,$(ASMOBJ_ARM))

# Options
INCLUDES        = -I . -I ./include $(patsubst %,-I%,$(PROJECT_INCLUDES))
DEFINES         = -D__$(MEM) -D__MCU=$(MCU) -D__FOSC=$(FOSC) $(patsubst %,-D%,$(PROJECT_DEFINES))
WARNINGSETTINGS = -Wall -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
                  -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused
ifneq ($(STACKWARNING),0)
WARNINGSETTINGS += -Wstack-usage=$(STACKWARNING)
endif
CFLAGS          = $(DBG) -pipe $(WARNINGSETTINGS) -mcpu=arm7tdmi -mtune=arm7tdmi\
                  -fno-builtin $(PROJECT_CFLAGS) $(INCLUDES) $(OPT) $(DEFINES)\
                  -MD -MP -MF depend/$(@F).d -fstack-usage
ASMFLAGS        = $(CFLAGS)
LDFLAGS         = -T$(LNK).out -nostartfiles -Wl,-Map=$(MAP)
SCRIPTFLAGS     = -E -P -traditional -undef -x assembler-with-cpp $(INCLUDES) -D__$(MEM) -D__MCU=$(MCU) -D__FOSC=$(FOSC)
ifeq ($(REMOVEUNUSED),TRUE)
CFLAGS         += -fdata-sections -ffunction-sections
LDFLAGS        += -Wl,-static,--gc-sections
endif
ifeq ($(USETHUMB),TRUE)
THUMB           = -mthumb
THUMB_INTERWORK = -mthumb-interwork
DEFINES        += -D__THUMB
endif
ifeq ($(GENERATELISTS),TRUE)
CFLAGS         += -Wa,-adhlns=$(subst $(suffix $<),.lst,$<)
endif

all: $(NAME)
ifeq ($(MEM),RUN_FROM_ROM)
	$(CC) $(SCRIPTFLAGS) -D__ELF=$(ELF) scripts/gdb_debugflash.script.pre -o scripts/gdb_debugflash.script
endif

$(OBJ): %.o: %.c Makefile
ifeq ($(USETHUMB),TRUE)
	@echo "Compiling $< in thumb mode.."
else
	@echo "Compiling $< in ARM mode.."
endif
	$(CC) $(CFLAGS) $(THUMB) $(THUMB_INTERWORK) -c $< -o $@

$(ASMOBJ): %.o: %.S Makefile
ifeq ($(USETHUMB),TRUE)
	@echo "Assembling $< in thumb mode.."
else
	@echo "Assembling $< in ARM mode.."
endif
	$(CC) $(ASMFLAGS) $(THUMB) $(THUMB_INTERWORK) -c $< -o $@

$(OBJ_ARM): %.o: %.c Makefile
	@echo "Compiling $< in ARM mode.."
	$(CC) $(CFLAGS) $(THUMB_INTERWORK) -c $< -o $@

$(ASMOBJ_ARM): %.o: %.S Makefile
	@echo "Assembling $< in ARM mode.."
	$(CC) $(ASMFLAGS) $(THUMB_INTERWORK) -c $< -o $@

$(NAME): $(ELF) $(HEX) $(BIN)
	@echo "Compilation for an $(MCU) MCU finished. Resulting size:"
	$(SIZE) -x $(ELF)
ifeq ($(MEM),RUN_FROM_RAM)
	@echo "Code is now compiled for RAM usage. Use 'make debug' to load and debug it"
else
ifeq ($(MEM),RUN_FROM_ROM)
	@echo "Code is now compiled for flash usage. Use 'make load' to load it, and if wanted, 'make debug' to debug it"
endif
endif

$(ELF): $(ASMOBJ) $(ASMOBJ_ARM) $(OBJ) $(OBJ_ARM) $(LNK)
	@echo "Linking.."
#	$(SIZE) -x -t $(OBJ)
	$(CC) -E -P -traditional -undef -x assembler-with-cpp $(INCLUDES) -D__$(MEM) -D__MCU=$(MCU) -D__FOSC=$(FOSC) -c $(LNK) -o $(LNK).out
	$(CC) $(THUMB_INTERWORK) $(LDFLAGS) $(filter-out %crt0.o,$(ASMOBJ_ARM)) $(ASMOBJ) $(OBJ) $(OBJ_ARM) $(PROJECT_LDFLAGS) $(LIBRARIES) -o $(ELF)

$(HEX): $(ELF)
ifeq ($(MEM),RUN_FROM_ROM)
	@echo "Creating HEX-file.."
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
endif

$(BIN): $(ELF)
ifeq ($(MEM),RUN_FROM_ROM)
	@echo "Creating BIN-file.."
	$(OBJCOPY) -O binary $(ELF) $(BIN)
endif

clean:
	@rm -f $(ASMOBJ) $(ASMOBJ_ARM) $(OBJ) $(OBJ_ARM) $(MAP) $(ELF) $(HEX) $(BIN) depend/*.d $(SU) $(LST) $(LNK).out scripts/gdb_debugflash.script scripts/gdb_loadflash.script scripts/openocd.cfg scripts/getsectors.sh crt0.o

ifeq ($(MEM),RUN_FROM_ROM)
openocd: $(BIN)
else
openocd: $(ELF)
endif
	$(CC) $(SCRIPTFLAGS) -D__JTAG_INTERFACE=$(JTAGINTERFACE) scripts/openocd.cfg.pre -o scripts/openocd.cfg
ifeq ($(MEM),RUN_FROM_ROM)
	@$(SHELL) scripts/startdebug.sh rom $(ELF) $(OPENOCD)
else
	@$(SHELL) scripts/startdebug.sh ram $(ELF) $(OPENOCD)
endif

stopopenocd:
	@$(SHELL) scripts/startdebug.sh stop

load: $(BIN)
ifeq ($(MEM),RUN_FROM_ROM)
	@echo "Preparing scripts for flashloading.."
	$(CC) $(SCRIPTFLAGS) -D__LOADFLASH -D__JTAG_INTERFACE=$(JTAGINTERFACE) scripts/openocd.cfg.pre -o scripts/openocd.cfg
	$(CC) $(SCRIPTFLAGS) scripts/getsectors.sh.pre -o scripts/getsectors.sh
	$(CC) $(SCRIPTFLAGS) -D__BIN=$(BIN) -D__SECTORS=`"$(SHELL)" scripts/getsectors.sh $(ELF) $(subst @,,$(SIZE))` scripts/gdb_loadflash.script.pre -o scripts/gdb_loadflash.script
	@echo "Starting openOCD for flashloading.."
	$(OPENOCD) -f scripts/openocd.cfg
# By touching the ELF, we wont get any "'debug' is up to date" messages here
	@touch $(ELF)
else
	@echo "Code has been compiled to be runned from RAM, can't load this..."
endif

gdb: $(ELF)
	$(GDB) -iex "set auto-load safe-path ./"

debug: $(ELF)
	$(CC) $(SCRIPTFLAGS) -D__JTAG_INTERFACE=$(JTAGINTERFACE) scripts/openocd.cfg.pre -o scripts/openocd.cfg
ifeq ($(MEM),RUN_FROM_ROM)
	@echo "Starting debugger (flash).."
	@echo ""
	@echo "NOTE: The program you are debugging right now must be loaded and running in flash!"
	@echo "      If that's not the case, stop now and run 'make load' first, and make sure"
	@echo "      the program runs"
	@echo ""
	@$(SHELL) scripts/startdebug.sh rom $(ELF) $(OPENOCD) $(INSIGHT)
else
ifeq ($(MEM),RUN_FROM_RAM)
	@echo "Starting debugger (RAM).."
	@$(SHELL) scripts/startdebug.sh ram $(ELF) $(OPENOCD) $(INSIGHT)
endif
endif
# By touching the ELF, we wont get any "'debug' is up to date" messages here
	@touch $(ELF)

doc: $(SRC) $(SRC_ARM) doc/doxygen/doxygen.conf doc/doxygen/mainpage
	$(DOXYGEN) doc/doxygen/doxygen.conf

sparse: $(SRC) $(SRC_ARM)
	$(SPARSE) $(INCLUDES) $(DEFINES)  $(filter-out $(wildcard drivers/*.S) crt0.S, $(SRC) $(SRC_ARM))

help:
	@echo "Following commands are available:"
	@echo "make / make all		Build all code"
	@echo "make debug		Start debugger"
	@echo "make load		Load program in flash"
	@echo "make openocd		Start openOCD, keep it running"
	@echo "make clean		Clean projectfiles"
	@echo "And some optional commands (not really required, only handy):"
	@echo "make doc		Build the doxygen documentation"
	@echo "make sparse		Check sourcecode with 'sparse'"
	@echo "NOTE: behavior of most of these commands depends on the settings in the"
	@echo "      Makefile; read the Makefile for the details."
	@echo "A more complete description of these commands can be found in the ALDS doxygen"
	@echo "documentation"

# Include the dependencies: We use '-include' here (note the '-'), which means that
# if the depend files don't exist, make will silently ignore the whole thing instead of throwing an error.
-include $(wildcard depend/*)