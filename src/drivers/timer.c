#include "timer.h"

#include <stdbool.h>

#include "clock.h"
#include "nrf52840_bitfields.h"

#define TIMER0_BASE 0x40008000U
#define TIMER1_BASE 0x40009000U
#define TIMER2_BASE 0x4000A000U
#define TIMER3_BASE 0x4001A000U
#define TIMER4_BASE 0x4001B000U

typedef struct {
  // (0x000) Start Timer
  volatile uint32_t TASKS_START;
  // (0x004) Stop Timer
  volatile uint32_t TASKS_STOP;
  // (0x008) Increment Timer (Counter mode only)
  volatile uint32_t TASKS_COUNT;
  // (0x00C) Clear time
  volatile uint32_t TASKS_CLEAR;
  // (0x010) Shut down timer
  volatile uint32_t TASKS_SHUTDOWN;
  volatile uint32_t __RESERVED[11];
  // (0x040) Capture Timer value to CC[n] register
  volatile uint32_t TASKS_CAPTURE[6];
  volatile uint32_t __RESERVED1[58];
  // (0x140) Compare event on CC[n] match
  volatile uint32_t EVENTS_COMPARE[6];
  volatile uint32_t __RESERVED2[42];
  // (0x200) Shortcuts between local events and tasks
  volatile uint32_t SHORTS;
  volatile uint32_t __RESERVED3[64];
  // (0x304) Enable interrupt
  volatile uint32_t INTENSET;
  // (0x308) Disable interrupt
  volatile uint32_t INTENCLR;
  volatile uint32_t __RESERVED4[126];
  // (0x504) Timer mode selection
  volatile uint32_t MODE;
  // (0x508) Configure the number of bits used by the TIMER
  volatile uint32_t BITMODE;
  volatile uint32_t __RESERVED5;
  // (0x510) Timer prescaler register
  volatile uint32_t PRESCALER;
  volatile uint32_t __RESERVED6[11];
  // (0x540) Capture/Compare register n
  volatile uint32_t CC[6];
} timer_t;

timer_t *const TIMER0 = (timer_t *)TIMER0_BASE;
timer_t *const TIMER1 = (timer_t *)TIMER1_BASE;
timer_t *const TIMER2 = (timer_t *)TIMER2_BASE;
timer_t *const TIMER3 = (timer_t *)TIMER3_BASE;
timer_t *const TIMER4 = (timer_t *)TIMER4_BASE;

timer_t *const timers[] = {TIMER0, TIMER1, TIMER2, TIMER3, TIMER4};

void init_timer(timer_id_t timer_id) {
  timer_t *timer = timers[timer_id];
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
void timer_start_timeout(timer_id_t timer_id, uint32_t us) {
  timer_t *timer = timers[timer_id];
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

bool timer_has_timeout_expired(timer_id_t timer_id) {
  if (timeout_expired) {
    return true;
  }

  timer_t *timer = timers[timer_id];
  if (timer->EVENTS_COMPARE[0]) {
    timer->TASKS_STOP = 1;
    timer->EVENTS_COMPARE[0] = 0;
    timeout_expired = true;
    return true;
  }

  return false;
}

void timer_stop_timeout(timer_id_t timer_id) {
  timer_t *timer = timers[timer_id];
  timer->TASKS_STOP = 1;
  timer->EVENTS_COMPARE[0] = 0;
  timeout_expired = true;
}
