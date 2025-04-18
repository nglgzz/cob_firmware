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
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -nostdlib -T nrf52.ld -Wl,-Map=final.map

OBJECTS= nrf52_startup.o main.o leds.o switches.o

all: $(OBJECTS) final.elf

# Syntax - targets ...: target-pattern: prereq-patterns ... In the case of the
# first target, foo.o, the target-pattern matches foo.o and sets the "stem" to
# be "foo". It then replaces the '%' in prereq-patterns with that stem
$(OBJECTS): %.o: %.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o *.elf *.map

flash: final.elf
	nrfutil device program --firmware final.elf

reset:
	nrfutil device reset

load:
	openocd -f /usr/share/openocd/scripts/board/nordic_nrf52_dk.cfg

debug:
	gdb-multiarch -tui
