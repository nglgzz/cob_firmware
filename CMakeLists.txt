cmake_minimum_required(VERSION 3.10)
project(mmk_firmware LANGUAGES C ASM CXX)

set(TARGET_ARM_MCU cortex-m4)

# Toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_LINKER arm-none-eabi-gcc)
# END - Toolchain


set(SOURCES
    src/gpio.c
    src/gpiote.c
    src/leds.c
    src/main.c
    src/nrf52_startup.c
    src/probe.c
    src/radio.c
    src/switches.c
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME}.elf ${SOURCES})

if(DEFINED ENV{RADIO_TX})
    add_definitions(-DRADIO_TX)
endif()

if(DEFINED ENV{RADIO_RX})
    add_definitions(-DRADIO_RX)
endif()

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
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=${TARGET_ARM_MCU}
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -std=gnu11
    -nostartfiles
    -nostdlib
    -Wall
    -O0
    -Wno-unused-variable
    -ggdb
)

# -nostartfiles			do not use the standard system startup files when linking
# -T					path to linker script
# -Wl,-Map=final.map	-Wl, linker specific arguments
target_link_options(${PROJECT_NAME}.elf PRIVATE
    -mcpu=${TARGET_ARM_MCU}
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv4-sp-d16
    -nostartfiles
    -nostdlib
    "-T${CMAKE_SOURCE_DIR}/scripts/nrf52.ld"
    "-Wl,-Map=${PROJECT_NAME}.map"
)
