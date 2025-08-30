#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keymap.h"
#include "keyscan.h"
#include "leds.h"
#include "usb_hid.h"
#include "usbd.h"

static uint8_t led_gpios[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint8_t);

#define N_LAYERS 2
#define N_SWITCHES 4
#define N_COLUMNS 2

static keymap_state_t device1 = {.n_layers = N_LAYERS,
                                 .n_switches = N_SWITCHES,
                                 .layers =
                                     {
                                         {KC_A, KC_S, KC_D, KC_F},
                                         {KC_Q, KC_W, KC_TRNS, MOD_LSFT},
                                     },
                                 .active_layers = 0x2,
                                 .previous_state = 0x0};

static keyscan_config_t config = {
    .gpios = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4},
    .gpios_len = 4,
    .cols_len = 2,
};

int example_keymap_hid() {
  init_usbd();
  init_device(0, device1);

  init_leds(led_gpios, led_gpios_len);
  init_keyscan_direct(0, &config);

  leds_blink();
  leds_set(1, 1);

  while (1) {
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_KEYMAP_HID
void KEYSCAN_EventHandler(uint8_t keyscan_id, keyscan_state_t state) {
  // This is a hack until update_states accepts keyscan_state_t
  uint32_t rows = state.rows[0] | (state.rows[1] << N_COLUMNS);
  hid_report_keyboard_t report = device_update_state(0, rows);
  leds_set_all(rows);

  hid_send_kb_report(&report);
}
#endif

