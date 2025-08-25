#include "probe.h"

#include <stdbool.h>

#include "gpio.h"
#include "leds.h"
#include "rtc.h"
#include "timer.h"
#include "utils.h"

static uint8_t profiler_pins[] = {PP_D0, PP_D1, PP_D2, PP_D3, PP_D4, PP_D5, PP_D6, PP_D7};
static uint8_t profiler_pins_len = sizeof(profiler_pins) / sizeof(profiler_pins[0]);
static probe_tag_t profiler_tags[8];
static bool probes_initialized = false;

void init_probes(probe_tag_t tags[], size_t size) {
  memcpy(profiler_tags, tags, size);
  profiler_pins_len = min(size, profiler_pins_len);

  for (int i = 0; i < profiler_pins_len; i++) {
    gpio_dir_pin_output(GPIO1, profiler_pins[i]);
    gpio_out_pin(GPIO1, profiler_pins[i], false);
  }

  init_timer(TIMER0);
  probes_initialized = true;
}

static inline int find_tag_pin(probe_tag_t tag) {
  if (!probes_initialized) return -1;

  for (int i = 0; i < profiler_pins_len; i++) {
    if (profiler_tags[i] == tag) {
      return profiler_pins[i];
    }
  }

  return -1;
}

void probe_on(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_out_pin(GPIO1, pin, true);
}

void probe_off(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_out_pin(GPIO1, pin, false);
}

void probe_pulse(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_out_pin(GPIO1, pin, true);
  timer_sleep_us(30);
  gpio_out_pin(GPIO1, pin, false);
}

void probe_pulse_times(probe_tag_t tag, uint32_t count) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  while (count--) {
    gpio_out_pin(GPIO1, pin, true);
    timer_sleep_us(30);
    gpio_out_pin(GPIO1, pin, false);

    if (count) {
      // This is to not have a redundant delay between the end of the
      // last pulse and the next instruction.
      timer_sleep_us(30);
    }
  }
}
