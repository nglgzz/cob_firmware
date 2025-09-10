#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  timer_id_sleep = 0,
  timer_id_radio,
  timer_id_keymap,
  timer_id_keyscan
} timer_id_t;

void init_timer(timer_id_t timer_id);
void timer_sleep_us(uint32_t us);
void timer_sleep_ms(uint32_t ms);
void timer_start_timeout(timer_id_t timer_id, uint32_t us);
bool timer_has_timeout_expired(timer_id_t timer_id);
void timer_stop_timeout(timer_id_t timer_id);

#endif  // TIMER_H
