/**
 * NOTES:
 *   - This example seems to be interfering with the SCR settings (sleeponexit, sleepdeep),
 *     disabling these settings makes this snippet work as intended.
 */
#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "leds.h"
#include "radio.h"
#include "utils.h"

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

static uint32_t report;

void send(uint32_t switches);

int example_radio_auto_tx() {
  init_leds(led_pins, led_pins_size);
  init_radio();

  leds_blink(500000);

#ifdef RADIO_RX
  leds_set(0, 1);

  while (1) {
    radio_receive(&report, sizeof(report));
    leds_set_all(report);
  }
#else
  while (1) {
    send(1);
    delay(5000000);

    send(2);
    delay(5000000);

    send(4);
    delay(10000000);

    send(8);
    delay(10000000);
  }
#endif

  while (1) {
    leds_set(3, 1);
    __asm__("WFI");
  }
}

void send(uint32_t switches) {
  radio_send(&switches, sizeof(switches));
  leds_set_all(switches);
}

