#include "timer.h"

#include <stdbool.h>

#include "clock.h"
#include "nrf52840_bitfields.h"

timer_t *const TIMER0 = (timer_t *)TIMER0_BASE;
timer_t *const TIMER1 = (timer_t *)TIMER1_BASE;
timer_t *const TIMER2 = (timer_t *)TIMER2_BASE;
timer_t *const TIMER3 = (timer_t *)TIMER3_BASE;
timer_t *const TIMER4 = (timer_t *)TIMER4_BASE;

void init_timer(timer_t *const timer) {
  clock_start_hfclk();

  timer->TASKS_STOP = 1;
  timer->TASKS_CLEAR = 1;

  // To calculate the effective frequency of the TIMER
  // fTIMER [MHz] = 16 / (2^PRESCALER)
  //
  // To calculate the PRESCALER based on the desired frequency of the TIMER
  // 2^PRESCALER = ROUND(16 / fTIMER [MHz])
  //
  // To calculate the counter period from the PRESCALER
  // PERIOD [us] = 1 / (16 / (2^PRESCALER))
  // PERIOD [us] = (2^PRESCALER) / 16
  //
  // To calculte the overflow time from the PERIOD
  // OVERFLOW [us] = PERIOD * (2^BITMODE)
  //
  // Based on desired frequency of 1MHz
  // (period 1us, overflow 4294967ms or 71m 34s approximately)
  timer->PRESCALER = 4;
  timer->BITMODE = TIMER_BITMODE_BITMODE_32Bit << TIMER_BITMODE_BITMODE_Pos;

  timer->INTENSET = TIMER_INTENSET_COMPARE0_Msk;
}

void timer_sleep_us(uint32_t us) {
  TIMER0->TASKS_STOP = 1;
  TIMER0->TASKS_CLEAR = 1;

  TIMER0->EVENTS_COMPARE[0] = 0;
  TIMER0->CC[0] = us;  // because 1 tick = 1 us

  TIMER0->TASKS_START = 1;
  while (TIMER0->EVENTS_COMPARE[0] == 0);
  TIMER0->EVENTS_COMPARE[0] = 0;

  TIMER0->TASKS_STOP = 1;
}

inline void timer_sleep_ms(uint32_t ms) { timer_sleep_us(ms * 1000); }

static bool timeout_expired = false;
void timer_start_timeout(timer_t *const timer, uint32_t us) {
  timer->TASKS_STOP = 1;
  timer->TASKS_CLEAR = 1;
  timeout_expired = false;

  // A value of zero is used to represent an infinite timeout, in such case we
  // clear the timer and don't start it, so the expired check will always be false.
  if (us == 0) return;

  timer->EVENTS_COMPARE[0] = 0;
  timer->CC[0] = us;  // because 1 tick = 1 us

  timer->TASKS_START = 1;
}

bool timer_has_timeout_expired(timer_t *const timer) {
  if (timeout_expired) {
    return true;
  }

  if (timer->EVENTS_COMPARE[0]) {
    timer->TASKS_STOP = 1;
    timer->EVENTS_COMPARE[0] = 0;
    timeout_expired = true;
    return true;
  }

  return false;
}

void timer_stop_timeout(timer_t *const timer) {
  timer->TASKS_STOP = 1;
  timer->EVENTS_COMPARE[0] = 0;
  timeout_expired = true;
}
