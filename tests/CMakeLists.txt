# Omitting toolchain as CMake will inferr it from the host system.

# Set source files
file(GLOB_RECURSE TEST_SOURCES ABSOLUTE "${CMAKE_SOURCE_DIR}/tests/*.c")
list(APPEND TEST_SOURCES "../src/keymap.c")

# Set directories that contain header files
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/drivers
    ${CMAKE_SOURCE_DIR}/src/examples
    ${CMAKE_SOURCE_DIR}/src/protocol
    ${CMAKE_SOURCE_DIR}/tests
)

add_executable(tests ${TEST_SOURCES})

#
# https://gcc.gnu.org/onlinedocs/gcc/index.html#toc-GCC-Command-Options
# https://gcc.gnu.org/onlinedocs/gcc/ARM-Options.html
#
# -c 					compile or assemble the source files, but do not link.
# -std=gnu11
# -nostdlib				no standard library
# -Wall					enable all warnings
# -O0					disables all optimizations
# -Wno-unused-variable
# -ggdb					produces debug information intended for gdb
target_compile_options(tests PRIVATE
    -std=gnu11
    -nostdlib
    -Wall
    -O0
    -Wno-unused-variable
    -ggdb
)
