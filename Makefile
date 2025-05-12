DIR_BUILD=build
DIR_SCRIPTS=scripts
FILE_EXECUTABLE=$(DIR_BUILD)/mmk_firmware.elf

.PHONY: build flash reset recover load debug format

all: build

build:
	cmake -S . -B build
	make --no-print-directory -C ./build clean all

flash: build
	nrfutil device program --firmware $(FILE_EXECUTABLE)
	nrfutil device reset

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
