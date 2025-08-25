#include <stdint.h>

#include "examples.h"
#include "leds.h"
#include "timer.h"

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static uint8_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

int example_blinky() {
  init_leds(led_pins, led_pins_size);

  leds_blink();

  while (1) {
    leds_blink();
  }
}
