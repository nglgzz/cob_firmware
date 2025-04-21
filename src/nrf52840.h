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
// NVIC
// -------

// Registers to enable and disable IRQ handlers. The bit position represents the
// ID of the interrupt to enable, with 0 being the handler on position 16 in the
// vector table (i.e. the first external IRQ handler).
#define NVIC_SETENA (*(volatile unsigned int *)0xE000E100)
#define NVIC_CLRENA (*(volatile unsigned int *)0xE000E180)

typedef enum {
  POWER_CLOCK_IRQn = 0,
  RADIO_IRQn = 1,
  UARTE0_UART0_IRQn = 2,
  SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQn = 3,
  SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQn = 4,
  NFCT_IRQn = 5,
  GPIOTE_IRQn = 6,
  SAADC_IRQn = 7,
  TIMER0_IRQn = 8,
  TIMER1_IRQn = 9,
  TIMER2_IRQn = 10,
  RTC0_IRQn = 11,
  TEMP_IRQn = 12,
  RNG_IRQn = 13,
  ECB_IRQn = 14,
  CCM_AAR_IRQn = 15,
  WDT_IRQn = 16,
  RTC1_IRQn = 17,
  QDEC_IRQn = 18,
  COMP_LPCOMP_IRQn = 19,
  SWI0_EGU0_IRQn = 20,
  SWI1_EGU1_IRQn = 21,
  SWI2_EGU2_IRQn = 22,
  SWI3_EGU3_IRQn = 23,
  SWI4_EGU4_IRQn = 24,
  SWI5_EGU5_IRQn = 25,
  TIMER3_IRQn = 26,
  TIMER4_IRQn = 27,
  PWM0_IRQn = 28,
  PDM_IRQn = 29,
  MWU_IRQn = 32,
  PWM1_IRQn = 33,
  PWM2_IRQn = 34,
  SPIM2_SPIS2_SPI2_IRQn = 35,
  RTC2_IRQn = 36,
  I2S_IRQn = 37,
  FPU_IRQn = 38
} IRQn_Type;

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

// Latch register indicating what GPIO pins that have met the criteria set in
// the PIN_CNF[n].SENSE registers. When reading 1 = criteria has been met, 0 =
// criteria has not been met. When writing 1 = clear latch
#define GPIO_LATCH (*(volatile unsigned int *)(GPIO_BASE + 0x520))

// Select between the default DETECT signal behavior and LDETECT mode
#define GPIO_DETECTMODE (*(volatile unsigned int *)(GPIO_BASE + 0x524))

// Bit number when setting values to the above registers
// Buttons
#define SW_PIN_1 11  // 0x5000072C
#define SW_PIN_2 12  // 0x50000730
#define SW_PIN_3 24  // 0x50000760
#define SW_PIN_4 25  // 0x50000764

#define SW_MASK (1 << SW_PIN_1 | 1 << SW_PIN_2 | 1 << SW_PIN_3 | 1 << SW_PIN_4)
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
#define PIN_CNF_DIR 0
#define PIN_CNF_INPUT 1
#define PIN_CNF_PULL 2
#define PIN_CNF_SENSE 16

#define CNF_SENSE_HIGH                                         \
  (0 << PIN_CNF_DIR | 0 << PIN_CNF_INPUT | 3 << PIN_CNF_PULL | \
   3 << PIN_CNF_SENSE)
#define CNF_SENSE_LOW                                          \
  (0 << PIN_CNF_DIR | 0 << PIN_CNF_INPUT | 3 << PIN_CNF_PULL | \
   2 << PIN_CNF_SENSE)

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
#define EVENTS_PORT \
  (*(volatile unsigned int *)(GPIOTE_BASE + 0x17C))  // 0x4000617C
// Enable interrupt (set desired pin to 1 to enable)
#define INTENSET (*(volatile unsigned int *)(GPIOTE_BASE + 0x304))
// Disable interrupt (set desired pin to 1 to disable)
#define INTENCLR (*(volatile unsigned int *)(GPIOTE_BASE + 0x308))

// Bit number representing the PORT event when setting INTEN* values.
#define PORT_EVENT 31

#endif  // NRF52840_H
