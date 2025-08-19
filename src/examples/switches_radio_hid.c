#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "leds.h"
#include "radio.h"
#include "switches.h"
#include "usb_hid.h"
#include "usbd.h"
#include "utils.h"

static uint8_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(uint8_t);

static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

int example_switches_radio_hid() {
#ifdef RADIO_RX
  init_usbd();
#endif

  init_leds(led_pins, led_pins_size);
  init_switches(switch_pins, switch_pins_size);
  init_radio();

  leds_blink(50000);

#ifdef RADIO_RX
  leds_set(0, 1);
  radio_receive();
#endif

  while (1) {
    leds_set(0, 0);
    __asm__("WFI");
  }
}

static radio_packet_t packet = {.len = 48, .data = {0}};
static uint32_t report;

#ifdef EXAMPLE_SWITCHES_RADIO_HID
void SWITCHES_PressedHandler(uint32_t switches) {
  report = switches;
  memcpy((void*)packet.data, &switches, sizeof(switches));
  radio_send(&packet);
}

void RADIO_SentHandler(volatile radio_packet_t* payload) { leds_set_all(report); }
void RADIO_ReceivedHandler(volatile radio_packet_t* payload) {
  if (RADIO->CRCSTATUS == 1) {
    memcpy((void*)&report, (void*)payload->data, sizeof(report));
    leds_set_all(report);
    hid_send_report(report);
  }
  radio_receive();
}
#endif
