// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

// These two are not used directly in this header file, instead they're used by
// leds.c that import this header file. stddef is defining size_t, while stdint
// is defining fixed width integer types.
#include <stddef.h>
#include <stdint.h>

// https://www.mouser.com/datasheet/2/297/nRF52840_OPS_v0.5-1074816.pdf
// (page 158)
// GPIO port 0
#define GPIO_BASE 0x50000000U

// Volatile tells the compiler to not optimize code related to this variable, as
// it's value can be modified by something other than the code that uses it.
// https://stackoverflow.com/a/246148
//
// Non volatile pointer to volatile data.
// https://stackoverflow.com/a/9736812
//
// GPIO output register: write GPIO port (1 sets pin to high, 0 to low)
#define GPIO_OUT (*(volatile unsigned int *)(GPIO_BASE + 0x504))

// GPIO output set register: set individual bits in GPIO port (1 sets pin to
// high, 0 has no effect)
#define GPIO_OUTSET (*(volatile unsigned int *)(GPIO_BASE + 0x508))

// GPIO output clear register: clear individual bits in GPIO port (1 sets pin to
// low, 0 has no effect)
#define GPIO_OUTCLR (*(volatile unsigned int *)(GPIO_BASE + 0x50C))

// GPIO direction register: direction of GPIO pins (0 set pin as input, 1 set
// pin as output)
#define GPIO_DIR (*(volatile unsigned int *)(GPIO_BASE + 0x514))

// Bit number when setting values to the above registers
#define LED_PIN_1 13
#define LED_PIN_2 14
#define LED_PIN_3 15
#define LED_PIN_4 16

void init_leds();
void blink_leds();

#endif  // LED_H