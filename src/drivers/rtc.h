#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC0_BASE 0x4000B000U
#define RTC1_BASE 0x40011000U
#define RTC2_BASE 0x40024000U

typedef struct {
  // (0x000) Start RTC COUNTER
  volatile uint32_t TASKS_START;
  // (0x004) Stop RTC COUNTER
  volatile uint32_t TASKS_STOP;
  // (0x008) Clear RTC COUNTER
  volatile uint32_t TASKS_CLEAR;
  // (0x00C) Set COUNTER to 0xFFFFF0
  volatile uint32_t TASKS_TRIGOVRFLW;
  volatile uint32_t __RESERVED[60];
  // (0x100) Event on COUNTER increment
  volatile uint32_t EVENTS_TICK;
  // (0x104) Event on COUNTER overflow
  volatile uint32_t EVENTS_OVRFLW;
  volatile uint32_t __RESERVED1[14];
  //(0x140) Compare event on CC[n] match
  volatile uint32_t EVENTS_COMPARE[4];
  volatile uint32_t __RESERVED2[109];
  // (0x304) Enable interrupt
  volatile uint32_t INTENSET;
  // (0x308) Disable interrupt
  volatile uint32_t INTENCLR;
  volatile uint32_t __RESERVED3[13];
  // (0x340) Enable or disable event routing
  volatile uint32_t EVTEN;
  // (0x344) Enable event routing
  volatile uint32_t EVTENSET;
  // (0x348) Disable event routing
  volatile uint32_t EVTENCLR;
  volatile uint32_t __RESERVED4[110];
  // (0x504) Current COUNTER value
  volatile uint32_t COUNTER;
  // (0x508) 12 bit prescaler for COUNTER frequency (32768/(PRESCALER+1)).
  // Must be written when RTC is stopped.
  volatile uint32_t PRESCALER;
  volatile uint32_t __RESERVED5[13];
  //(0x540) Compare register n (CC[3] not available on RTC0).
  volatile uint32_t CC[4];
} rtc_t;

extern rtc_t *const RTC0;
extern rtc_t *const RTC1;
extern rtc_t *const RTC2;

void init_rtc();
void rtc_sleep_ms(uint32_t ms);

#endif  // RTC_H
