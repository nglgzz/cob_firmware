#include "timer.h"

#include <time.h>

void init_timer(timer_id_t timer_id) {}
void timer_sleep_us(uint32_t us) {}
void timer_sleep_ms(uint32_t ms) {}

clock_t timers[10] = {};
void timer_start_timeout(timer_id_t timer_id, uint32_t us) {
  clock_t time_now_in_us = clock() / (CLOCKS_PER_SEC * 1e6);
  timers[timer_id] = (time_now_in_us) + us;
}

bool timer_has_timeout_expired(timer_id_t timer_id) {
  clock_t time_now_in_us = clock() / (CLOCKS_PER_SEC * 1e6);
  return time_now_in_us > timers[timer_id];
}

void timer_stop_timeout(timer_id_t timer_id) { timers[timer_id] = 0; }
