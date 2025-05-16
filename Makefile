DIR_BUILD=build
DIR_SCRIPTS=scripts
FILE_EXECUTABLE=$(DIR_BUILD)/mmk_firmware.elf

.PHONY: build build-rx build-tx flash flash-rx flash-tx reset recover load debug format

all: build

build:
	cmake -S . -B build
	make --no-print-directory -C ./build clean all
build-rx:
	RADIO_RX=1 cmake -S . -B build-rx
	make --no-print-directory -C ./build-rx clean all
build-tx:
	RADIO_TX=1 cmake -S . -B build-tx
	make --no-print-directory -C ./build-tx clean all

flash: build
	nrfutil device program --firmware $(FILE_EXECUTABLE)
	nrfutil device reset
flash-rx: build-rx
	nrfutil device program  --serial-number 1050232668 --firmware build-rx/mmk_firmware.elf
	nrfutil device reset --serial-number 1050232668
flash-tx: build-tx
	nrfutil device program  --serial-number 1050276985 --firmware build-tx/mmk_firmware.elf
	nrfutil device reset --serial-number 1050276985

reset:
	nrfutil device reset
reset-rx:
	nrfutil device reset  --serial-number 1050232668
reset-tx:
	nrfutil device reset  --serial-number 1050276985

recover:
	nrfutil device recover
recover-rx:
	nrfutil device recover  --serial-number 1050232668
recover-tx:
	nrfutil device recover  --serial-number 1050276985


load:
	openocd -f /usr/share/openocd/scripts/board/nordic_nrf52_dk.cfg

debug:
	gdb-multiarch -tui --command $(DIR_SCRIPTS)/.gdbinit

format:
	clang-format -i src/*
