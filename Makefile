DIR_BUILD=build
DIR_SCRIPTS=scripts
FILE_EXECUTABLE=$(DIR_BUILD)/mmk_firmware.elf

# Need to update this on launch.json too
BOARD_1=1050232668
BOARD_2=1050276985

RX_BOARD=$(BOARD_2)
TX_BOARD=$(BOARD_1)


.PHONY:  build-rx build-tx  flash-rx flash-tx reset-rx reset-tx recover-rx reset-tx load debug format

all: build-rx build-tx flash-rx flash-tx

build-rx:
	RADIO_RX=1 cmake -S . -B build-rx
	make --no-print-directory -C ./build-rx clean all
build-tx:
	RADIO_TX=1 cmake -S . -B build-tx
	make --no-print-directory -C ./build-tx clean all

flash-rx: build-rx
	nrfutil device program  --serial-number $(RX_BOARD) --firmware build-rx/mmk_firmware.elf
	nrfutil device reset --serial-number $(RX_BOARD)
flash-tx: build-tx
	nrfutil device program  --serial-number $(TX_BOARD) --firmware build-tx/mmk_firmware.elf
	nrfutil device reset --serial-number $(TX_BOARD)

reset-rx:
	nrfutil device reset  --serial-number $(RX_BOARD)
reset-tx:
	nrfutil device reset  --serial-number $(TX_BOARD)

recover-rx:
	nrfutil device recover  --serial-number $(RX_BOARD)
recover-tx:
	nrfutil device recover  --serial-number $(TX_BOARD)


load:
	openocd -f /usr/share/openocd/scripts/board/nordic_nrf52_dk.cfg

debug:
	gdb-multiarch -tui --command $(DIR_SCRIPTS)/.gdbinit

format:
	clang-format -i $$(find ./src -type f)
