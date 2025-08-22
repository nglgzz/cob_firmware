#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "leds.h"
#include "probe.h"
#include "radio.h"
#include "switches.h"
#include "utils.h"

static uint8_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(uint8_t);

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

static uint32_t report;

int example_radio_leds() {
  init_leds(led_pins, led_pins_size);
  init_radio();

  leds_blink(50000);

#ifdef RADIO_RX
  leds_set(0, 1);

  while (1) {
    radio_receive(&report, sizeof(report));
    leds_set_all(report);
  }
#endif
  init_switches(switch_pins, switch_pins_size);

  while (1) {
    leds_set(3, 1);
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_RADIO_LEDS
void SWITCHES_PressedHandler(uint32_t switches) {
  probe_on(probe_tag_switch_handler);
  radio_send(&switches, sizeof(switches));
  leds_set_all(switches);
  probe_off(probe_tag_switch_handler);
}
#endif
