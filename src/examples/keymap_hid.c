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

static keyscan_config_t scan_config = {
    .gpios = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4},
    .gpios_len = 4,
    .cols_len = 2,
};

static uint32_t layout[] = {
    L0(0, 0, 0),
    L0(0, 0, 1),
    L0(0, 1, 0),
    L0(0, 1, 1),
};
static const uint8_t layout_len = sizeof(layout) / sizeof(layout[0]);

static uint16_t layers[N_LAYERS][N_SWITCHES] = {
    {KC_A, KC_S, KC_D, KC_F},
    {KC_Q, KC_W, KC_TRNS, MOD_LSFT},
};
static const uint8_t layers_len = sizeof(layers) / sizeof(layers[0]);

static keymap_config_t keymap_config = {
    .layout_len = layout_len,
    .layout = layout,

    .layers_len = layers_len,
    .layers = layers[0],
};

int example_keymap_hid() {
  init_usbd();
  init_device(0, keymap_config);

  init_leds(led_gpios, led_gpios_len);
  init_keyscan_direct(0, &scan_config);

  leds_blink();
  leds_set(1, 1);

  while (1) {
    __asm__("WFI");
  }
}

void KEYSCAN_EventHandler(uint8_t keyscan_id, keyscan_state_t state) {
  // This is a hack until update_states accepts keyscan_state_t
  uint32_t matrix = state.matrix[0] | (state.matrix[1] << N_COLUMNS);
  hid_report_keyboard_t report = device_update_state(0, matrix);
  leds_set_all(matrix);

  hid_send_kb_report(&report);
}
#ifdef EXAMPLE_KEYMAP_HID
#endif

