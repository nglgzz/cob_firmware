#include <stddef.h>
#include <stdint.h>

#include "device.h"
#include "examples.h"
#include "keyscan.h"
#include "leds.h"
#include "usb_hid.h"
#include "usbd.h"

static uint8_t switch_gpios[] = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4};
static size_t switch_gpios_len = sizeof(switch_gpios) / sizeof(uint8_t);

static uint8_t led_gpios[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint8_t);

#define N_LAYERS 2
#define N_SWITCHES 4
#define N_COLUMNS 2

static device_state_t device1 = {.n_layers = N_LAYERS,
                                 .n_switches = N_SWITCHES,
                                 .layers =
                                     {
                                         {KC_A, KC_S, KC_D, KC_F},
                                         {KC_Q, KC_W, KC_TRNS, MOD_LSFT},
                                     },
                                 .active_layers = 0x2,
                                 .previous_state = 0x0};

int example_keymap_hid() {
  init_usbd();
  init_device(0, device1);

  init_leds(led_gpios, led_gpios_len);
  init_keyscan_direct(switch_gpios, switch_gpios_len, N_COLUMNS);

  leds_blink();
  leds_set(1, 1);

  while (1) {
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_KEYMAP_HID
void KEYSCAN_EventHandler(keyscan_t keyscan) {
  // This is a hack until update_states accepts keyscan_t
  uint32_t rows = keyscan.rows[0] | (keyscan.rows[1] << N_COLUMNS);
  hid_report_keyboard_t report = device_update_state(0, rows);
  leds_set_all(rows);

  hid_send_kb_report(&report);
}
#endif

