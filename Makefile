DIR_SCRIPTS=scripts
FILE_EXECUTABLE=src/mmk_firmware.elf

# Need to update this on launch.json too
BOARD_1=1050232668
BOARD_2=1050276985

.PHONY:  build1 build2 build_test flash1 flash2 reset1 reset2 recover1 reset2 load debug format

all: build1 build2 flash1 flash2

build1:
	RADIO_RX=1 cmake -S . -B build1
	make --no-print-directory -C ./build1 clean all
build2:
	RADIO_TX=1 cmake -S . -B build2
	make --no-print-directory -C ./build2 clean all

# build1:
# 	RADIO_TX=1 cmake -S . -B build1
# 	make --no-print-directory -C ./build1 clean all
# build2:
# 	RADIO_RX=1 cmake -S . -B build2
# 	make --no-print-directory -C ./build2 clean all

build_test:
	cmake -S . -B build_test
	make --no-print-directory -C ./build_test clean tests

flash1: build1
	nrfutil device program  --serial-number $(BOARD_1) --firmware build1/$(FILE_EXECUTABLE)
	nrfutil device reset --serial-number $(BOARD_1)
flash2: build2
	nrfutil device program  --serial-number $(BOARD_2) --firmware build2/$(FILE_EXECUTABLE)
	nrfutil device reset --serial-number $(BOARD_2)

test: build_test
	./build_test/tests/tests

reset1:
	nrfutil device reset  --serial-number $(BOARD_1)
reset2:
	nrfutil device reset  --serial-number $(BOARD_2)

recover1:
	nrfutil device recover  --serial-number $(BOARD_1)
recover2:
	nrfutil device recover  --serial-number $(BOARD_2)


load:
	openocd -f /usr/share/openocd/scripts/board/nordic_nrf52_dk.cfg

debug:
	gdb-multiarch -tui --command $(DIR_SCRIPTS)/.gdbinit

format:
	clang-format -i $$(find ./src -type f)
