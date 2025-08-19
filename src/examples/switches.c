#include "switches.h"

#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "leds.h"

static uint8_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(uint8_t);

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

int example_switches() {
  init_leds(led_pins, led_pins_size);
  init_switches(switch_pins, switch_pins_size);

  while (1) {
    toggle_led(0, 0);
    __asm__("WFI");
  }
}
