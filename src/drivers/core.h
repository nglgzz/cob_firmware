#ifndef CORE_H
#define CORE_H

#include <stdint.h>

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
  FPU_IRQn = 38,
  USBD_IRQn = 39,
  UARTE1_IRQn = 40,
  QSPI_IRQn = 41,
  CRYPTOCELL_IRQn = 42,
  PWM3_IRQn = 45,
  SPIM3_IRQn = 47
} IRQn_t;

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

void NVIC_EnableIRQ(IRQn_t IRQn);
void NVIC_DisableIRQ(IRQn_t IRQn);
void NVIC_SetPriority(IRQn_t IRQn, uint8_t priority);

#endif  // CORE_H
