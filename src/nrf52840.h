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

// Volatile tells the compiler to not optimize code related to this variable, as
// it's value can be modified by something other than the code that uses it.
// https://stackoverflow.com/a/246148
//
// Non volatile pointer to volatile data.
// https://stackoverflow.com/a/9736812
//

// -------
// GPIOTE
// -------
// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpiote.html
#define GPIOTE_BASE 0x40006000U

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
