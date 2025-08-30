#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"
#include "radio.h"
#include "radio_arq.h"
#include "usb_hid.h"
#include "usbd.h"
#include "utils.h"

static uint8_t switch_gpios[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_gpios_size = sizeof(switch_gpios) / sizeof(uint8_t);

static uint8_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);

static uint32_t report;

int example_radio_arq_hid() {
#ifdef RADIO_RX
  init_usbd();
#endif

  init_leds(led_gpios, led_gpios_size);
  init_keyscan_direct(switch_gpios, switch_gpios_size, switch_gpios_size);
  init_radio();

  leds_blink();

#ifdef RADIO_RX
  leds_set(0, 1);

  while (1) {
    radio_arq_receive(&report, sizeof(report));
    leds_set_all(report);
    hid_send_report(report);
  }
#endif

  while (1) {
    leds_set(3, 1);
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_RADIO_ARQ_HID
void KEYSCAN_EventHandler(keyscan_t keyscan) {
  radio_arq_send(&keyscan.rows[0], sizeof(keyscan.rows[0]));
  leds_set_all(keyscan.rows[0]);
}
#endif
