#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"

static uint8_t switch_gpios[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_gpios_size = sizeof(switch_gpios) / sizeof(uint8_t);

static uint8_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);

int example_switches_leds() {
  init_leds(led_gpios, led_gpios_size);
  init_keyscan_direct(switch_gpios, switch_gpios_size, switch_gpios_size);

  leds_blink();
  leds_set(0, 1);

  while (1) {
    __asm__("WFI");
  }
}
#ifdef EXAMPLE_SWITCHES_LEDS
void KEYSCAN_EventHandler(keyscan_t keyscan) { leds_set_all(keyscan.rows[0]); }
#endif
