#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"

static uint16_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint16_t);

static keyscan_gpios_t matrix = {
    .direct = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4},
    .direct_len = 4,
};

int example_switches_leds() {
  init_leds(led_gpios, led_gpios_len);
  init_keyscan_direct(0, &matrix);

  leds_blink();
  leds_set(0, 1);

  while (1) {
    __asm__("WFI");
  }
}
#ifdef EXAMPLE_SWITCHES_LEDS
void KEYSCAN_EventHandler(uint8_t keyscan_id, keyscan_state_t state) {
  leds_set_all(state.matrix[0]);
}
#endif
