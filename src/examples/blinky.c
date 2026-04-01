#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "leds.h"
#include "timer.h"

static uint16_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint16_t);

int example_blinky() {
  init_leds(led_gpios, led_gpios_len);

  leds_blink();

  while (1) {
    leds_blink();
  }
}
