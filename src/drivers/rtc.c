
#include "rtc.h"

#include "clock.h"
#include "nrf52840_bitfields.h"

rtc_t *const RTC0 = (rtc_t *)RTC0_BASE;
rtc_t *const RTC1 = (rtc_t *)RTC1_BASE;
rtc_t *const RTC2 = (rtc_t *)RTC2_BASE;

void init_rtc() {
  clock_start_lfclk();

  RTC0->TASKS_STOP = 1;
  RTC0->TASKS_CLEAR = 1;

  // To calculate the effective frequency of the RTC
  // fRTC [kHz] = 32.768 / (PRESCALER + 1 )
  //
  // To calculate the PRESCALER based on the desired frequency of the RTC
  // PRESCALER = ROUND(32.768 / fRTC [kHz]) - 1
  //
  // To calculate the counter period from the PRESCALER
  // PERIOD [ms] = 1 / (32.768 / (PRESCALER + 1))
  // PERIOD [ms] = (PRESCALER + 1) / 32.768
  //
  // To calculte the overflow time from the PERIOD
  // OVERFLOW [ms] = PERIOD * (2^24)
  //
  // Based on desired frequency of 2kHz
  // (period 30.517 us, overflow 512s)
  RTC0->PRESCALER = 0;

  RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
}

void rtc_sleep_ms(uint32_t ms) {
  // To calculate the number of ticks needed for the desired duration in ms.
  // TICKS = ms / PERIOD
  //
  // Or using the prescaler instead of the period
  // TICKS = ms * 32.768 / (PRESCALER + 1)
  uint32_t ticks = ms * 32768UL / (RTC0->PRESCALER + 1) / 1000UL;

  // If ticks is 0, the COMPARE event is never triggered.
  if (ticks == 0) ticks = 1;

  // RTC has 24 bits, so this is a guard to not overflow.
  if (ticks > 0xFFFFFF) ticks = 0xFFFFFF;

  RTC0->TASKS_STOP = 1;
  RTC0->TASKS_CLEAR = 1;
  RTC0->EVENTS_COMPARE[0] = 0;

  RTC0->CC[0] = ticks;
  RTC0->TASKS_START = 1;

  while (RTC0->EVENTS_COMPARE[0] == 0);

  RTC0->TASKS_STOP = 1;
  RTC0->TASKS_CLEAR = 1;
  RTC0->EVENTS_COMPARE[0] = 0;
}
