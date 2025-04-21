########################################
#  SOURCE AND BUILD ARTIFACTS
########################################

DIR_SRC=src
DIR_BUILD=build
DIR_BIN=bin
DIR_SCRIPTS=scripts

FILES_SRC=$(wildcard $(DIR_SRC)/*.c)
FILES_OBJ=$(patsubst $(DIR_SRC)/%.c,$(DIR_BUILD)/%.o,$(FILES_SRC))
FILE_EXECUTABLE=$(DIR_BIN)/final.elf

########################################
#  COMPILER AND TARGET CONFIGURATION
########################################

CC=arm-none-eabi-gcc
MACH=cortex-m4
#
# https://gcc.gnu.org/onlinedocs/gcc/index.html#toc-GCC-Command-Options
# https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
#
# -c 					compile or assemble the source files, but do not link.
# -mcpu					target ARM processor
# -mthumb				generate code that executes in thumb state (cortex M
# 						processor only support Thumb state, not ARM state).
# -mfloat-abi=hard		Specifies which floating-point ABI to use. ‘hard’ allows
# 						generation of floating-point instructions and uses
# 						FPU-specific calling conventions.
# -mfpu=fpv4-sp-d16		Specifies what floating point hardware is available on the target.
# -std=gnu11
# -nostartfiles			do not use the standard system startup files when linking
# -nostdlib				no standard library
# -Wall					enable all warnings
# -O0					disables all optimizations
# -Wno-unused-variable
# -ggdb					produces debug information intended for gdb
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -nostartfiles -nostdlib -Wall -O0 -Wno-unused-variable -ggdb

# -nostartfiles			do not use the standard system startup files when linking
# -T					path to linker script
# -Wl,-Map=final.map	-Wl, linker specific arguments
<<<<<<< HEAD
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -nostdlib -T $(DIR_SRC)/nrf52.ld -Wl,-Map=$(DIR_BIN)/final.map
=======
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -nostdlib -T $(DIR_SCRIPTS)/nrf52.ld -Wl,-Map=$(DIR_BIN)/final.map
>>>>>>> 99ef47e (fixup! reorganize repo)


########################################
#  BUILD
########################################

all: $(FILES_OBJ) $(FILE_EXECUTABLE)

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(FILE_EXECUTABLE): $(FILES_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(DIR_BUILD)/*.o $(DIR_BIN)/*.elf $(DIR_BIN)/*.map


 ########################################
#  MCU HELPERS
########################################

flash: $(FILE_EXECUTABLE)
	nrfutil device program --firmware $(FILE_EXECUTABLE)

reset:
	nrfutil device reset

recover:
	nrfutil device recover

load:
	openocd -f /usr/share/openocd/scripts/board/nordic_nrf52_dk.cfg

debug:
	gdb-multiarch -tui --command $(DIR_SCRIPTS)/.gdbinit

format:
	clang-format -i src/*
