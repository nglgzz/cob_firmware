#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

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

// Used for probes
extern timer_t *const TIMER0;
// Used for radio
extern timer_t *const TIMER1;
// Used for ARQ
extern timer_t *const TIMER2;
extern timer_t *const TIMER3;
extern timer_t *const TIMER4;

void init_timer(timer_t *const timer);
void timer_sleep_us(timer_t *const timer, uint32_t us);
void timer_start_timeout(timer_t *const timer, uint32_t us);
bool timer_has_timeout_expired(timer_t *const timer);

#endif  // TIMER_H
