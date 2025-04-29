#ifndef CORE_H

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

/**
 * System control register:
 * [4] SEVONPEND    Send event on pending bit
 * [3] -
 * [2] SLEEPDEEP    Controls whether the processor uses sleep or deep sleep as
 *                  it's low power mode
 * [1] SLEEPONEXIT  Indicates sleep-on-exit when returning from handler mode to
 *                  thread mode
 * [0] -            Reserved
 */
#define SCR (*(volatile unsigned int *)0xE000ED10)

#define CORE_H
#endif  // CORE_H
