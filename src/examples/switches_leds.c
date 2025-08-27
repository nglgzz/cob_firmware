#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"

static uint8_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(uint8_t);

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

int example_switches_leds() {
  init_leds(led_pins, led_pins_size);
  init_keyscan(switch_pins, switch_pins_size);

  leds_blink();

  while (1) {
    leds_set(0, 0);
    __asm__("WFI");
  }
}
#ifdef EXAMPLE_SWITCHES_LEDS
void KEYSCAN_ToggleHandler(uint32_t switches) { leds_set_all(switches); }
#endif
