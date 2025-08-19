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

void send(uint32_t switches);

int example_radio_auto_tx() {
  init_leds(led_pins, led_pins_size);
  init_radio();

  leds_blink(500000);

#ifdef RADIO_RX
  leds_set(0, 1);
  radio_receive();
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
    leds_set(0, 0);
    __asm__("WFI");
  }
}

static radio_packet_t packet = {.len = 48, .data = {0}};
static uint32_t report;

void send(uint32_t switches) {
  report = switches;
  memcpy((void*)packet.data, &switches, sizeof(switches));
  radio_send(&packet);
}

#ifdef EXAMPLE_RADIO_AUTO_TX
void RADIO_SentHandler(volatile radio_packet_t* payload) { leds_set_all(report); }
void RADIO_ReceivedHandler(volatile radio_packet_t* payload) {
  if (RADIO->CRCSTATUS == 1) {
    memcpy((void*)&report, (void*)payload->data, sizeof(report));
    leds_set_all(report);
  }
  radio_receive();
}
#endif
