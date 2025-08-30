#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"
#include "probe.h"
#include "radio.h"
#include "utils.h"

static uint8_t switch_gpios[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_gpios_size = sizeof(switch_gpios) / sizeof(uint8_t);

static uint8_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);

static uint32_t report;

int example_radio_leds() {
  init_leds(led_gpios, led_gpios_size);
  init_radio();

  leds_blink();

#ifdef RADIO_RX
  leds_set(0, 1);

  while (1) {
    radio_receive(&report, sizeof(report));
    leds_set_all(report);
  }
#endif
  init_keyscan_direct(switch_gpios, switch_gpios_size, switch_gpios_size);

  while (1) {
    leds_set(3, 1);
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_RADIO_LEDS
void KEYSCAN_EventHandler(keyscan_t keyscan) {
  probe_pulse_times(probe_tag_switch_handler, 3);
  radio_send(&keyscan.rows[0], sizeof(keyscan.rows[0]));
  leds_set_all(keyscan.rows[0]);
  probe_pulse_times(probe_tag_switch_handler, 1);
}
#endif
