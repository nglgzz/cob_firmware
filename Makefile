CC=arm-none-eabi-gcc
MACH=cortex-m4
# TODO: What are these flags?
# https://gcc.gnu.org/onlinedocs/gcc/index.html#toc-GCC-Command-Options
# https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html

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
# -O0
# -Wno-unused-variable
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -std=gnu11 -nostartfiles -nostdlib -Wall -O0 -Wno-unused-variable

# -nostartfiles			do not use the standard system startup files when linking
# -T					path to linker script
# -Wl,-Map=final.map	-Wl, linker specific arguments
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -nostartfiles -nostdlib -T nrf52.ld -Wl,-Map=final.map

all: main.o leds.o startup.o final.elf

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

leds.o: leds.c
	$(CC) $(CFLAGS) -o $@ $^

startup.o: nrf52_startup.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: main.o leds.o startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.o *.elf *.map

# TODO: how does jlink work??
flash:
	echo "r\nloadfile final.elf\nr\nq" | JLinkExe -device nRF52840_xxAA -if SWD -speed 4000