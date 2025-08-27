#include <stddef.h>
#include <stdint.h>

#include "device.h"
#include "examples.h"
#include "keyscan.h"
#include "leds.h"
#include "usb_hid.h"
#include "usbd.h"

static uint8_t switch_pins[] = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(uint8_t);

static uint8_t led_pins[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
static size_t led_pins_size = sizeof(led_pins) / sizeof(uint8_t);

#define N_LAYERS 2
#define N_SWITCHES 4

static device_state_t device1 = {.n_layers = N_LAYERS,
                                 .n_switches = N_SWITCHES,
                                 .layers =
                                     {
                                         {KC_A, KC_S, KC_D, KC_F},
                                         {KC_Q, KC_W, KC_TRNS, MOD_LSFT},
                                     },
                                 .active_layers = 0x2,
                                 .previous_state = 0x0};

int example_simple_hid() {
  init_usbd();
  init_device(0, device1);

  init_leds(led_pins, led_pins_size);
  init_keyscan(switch_pins, switch_pins_size);

  leds_blink();

  while (1) {
    leds_set(1, 1);
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_SIMPLE_HID
void KEYSCAN_ToggleHandler(uint32_t switches) {
  hid_report_keyboard_t report = device_update_state(0, switches);
  hid_send_kb_report(&report);
  leds_set_all(switches);
}
#endif
