#include "probe.h"

#include "gpio.h"
#include "leds.h"

static uint8_t profiler_pins[] = {PP_D0, PP_D1, PP_D2, PP_D3, PP_D4, PP_D5};
static uint8_t profiler_pins_len = sizeof(profiler_pins) / sizeof(profiler_pins[0]);

void init_probes() {
  for (int i = 0; i < profiler_pins_len; i++) {
    gpio_dir_pin_output(GPIO1, profiler_pins[i]);
    gpio_out_pin(GPIO1, profiler_pins[i], false);
  }
}

void probe_pulse(uint8_t pin) {
  gpio_out_pin(GPIO1, pin, true);
  delay(60);  // Add a small delay
  gpio_out_pin(GPIO1, pin, false);
}
void probe_on(uint8_t pin) { gpio_out_pin(GPIO1, pin, true); }
void probe_off(uint8_t pin) { gpio_out_pin(GPIO1, pin, false); }
