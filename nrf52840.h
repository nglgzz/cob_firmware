// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef NRF52840_H
#define NRF52840_H

// These two are not used directly in this header file, instead they're used by
// leds.c that import this header file. stddef is defining size_t, while stdint
// is defining fixed width integer types.
#include <stddef.h>
#include <stdint.h>

// -------
// GPIO
// -------

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
// Buttons
#define SW_PIN_1 11
#define SW_PIN_2 12
#define SW_PIN_3 24
#define SW_PIN_4 25

// Leds
#define LED_PIN_1 13
#define LED_PIN_2 14
#define LED_PIN_3 15
#define LED_PIN_4 16

// Configuration for pin 0
#define PIN_CNF_OFFSET 0x700U
// Configuration size
#define PIN_CNF_SIZE 4U
/**
 * PIN_CNF[n] - Configuration of GPIO pin `n`:
 *
 * - A[0] DIR: 0 = input, 1 = output (same as DIR register)
 * - B[1] INPUT: 0 = connect input buffer, 1 =  disconnect input buffer
 * - C[2,3] PULL: 0 = no pull, 1 = pulldown, 3 = pullup
 * - D[8,9,10] DRIVE: ...
 * - E[16,17] SENSE: 0 = disabled, 2 = sense high level, 3 = sense low level
 */
#define PIN_CNF(pin)                                       \
  (*(volatile unsigned int *)(GPIO_BASE + PIN_CNF_OFFSET + \
                              (pin * PIN_CNF_SIZE)))

// Bit to set for configuration
#define PIN_CNF_PULL 2
#define PIN_CNF_SENSE 16

#define CNF_SENSE_HIGH (3 << PIN_CNF_PULL | 2 << PIN_CNF_SENSE)
#define CNF_SENSE_LOW (3 << PIN_CNF_PULL | 3 << PIN_CNF_SENSE)

// -------
// GPIOTE
// -------
// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpiote.html
#define GPIOTE_BASE 0x40006000U

/**
 * In order to prevent spurious interrupts from the PORT event while configuring
 * the sources, the following must be performed:
 *
 *   1. Disable interrupts on the PORT event (through INTENCLR.PORT).
 *   2. Configure the sources (PIN_CNF[n].SENSE).
 *   3. Clear any potential event that could have occurred during configuration
 *      (write '0' to EVENTS_PORT).
 *   4. Enable interrupts (through INTENSET.PORT).
 */

// Event generated from multiple input GPIO pins with SENSE mechanism enabled
// (1 = event generated, 0 = event not generated, only one bit is used).
#define EVENTS_PORT (*(volatile unsigned int *)(GPIOTE_BASE + 0x17C))
// Enable interrupt (set desired pin to 1 to enable)
#define INTENSET (*(volatile unsigned int *)(GPIOTE_BASE + 0x304))
// Disable interrupt (set desired pin to 1 to disable)
#define INTENCLR (*(volatile unsigned int *)(GPIOTE_BASE + 0x308))

// Bit number representing the PORT event when setting INTEN* values.
#define PORT_EVENT 31

#endif  // NRF52840_H