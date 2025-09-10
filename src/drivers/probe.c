#include "probe.h"

#include <stdbool.h>

#include "gpio.h"
#include "leds.h"
#include "nrf52840_bitfields.h"
#include "rtc.h"
#include "timer.h"
#include "utils.h"

static uint8_t profiler_gpios[] = {PP_D0, PP_D1, PP_D2, PP_D3, PP_D4, PP_D5, PP_D6, PP_D7};
static uint8_t profiler_gpios_len = sizeof(profiler_gpios) / sizeof(profiler_gpios[0]);
static probe_tag_t profiler_tags[8];
bool probes_initialized = false;

void init_probes(probe_tag_t tags[], size_t size) {
  memcpy(profiler_tags, tags, size);
  profiler_gpios_len = min(size, profiler_gpios_len);

  for (int i = 0; i < profiler_gpios_len; i++) {
    gpio_mode(1, profiler_gpios[i], GPIO_DIR_PIN0_Output);
    gpio_write(1, profiler_gpios[i], 0);
  }

  init_timer(TIMER0);
  probes_initialized = true;
}

static inline int find_tag_pin(probe_tag_t tag) {
  if (!probes_initialized) return -1;

  for (int i = 0; i < profiler_gpios_len; i++) {
    if (profiler_tags[i] == tag) {
      return profiler_gpios[i];
    }
  }

  return -1;
}

void probe_on(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_write(1, pin, 1);
}

void probe_off(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_write(1, pin, 0);
}

void probe_pulse(probe_tag_t tag) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  gpio_write(1, pin, 1);
  timer_sleep_us(30);
  gpio_write(1, pin, 0);
}

void probe_pulse_times(probe_tag_t tag, uint32_t count) {
  int pin = find_tag_pin(tag);
  if (pin < 0) return;

  while (count--) {
    gpio_write(1, pin, 1);
    timer_sleep_us(30);
    gpio_write(1, pin, 0);

    if (count) {
      // This is to not have a redundant delay between the end of the
      // last pulse and the next instruction.
      timer_sleep_us(30);
    }
  }
}
