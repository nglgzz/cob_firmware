#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keymap.h"
#include "keyscan.h"
#include "leds.h"
#include "timer.h"
#include "usb_hid.h"
#include "usbd.h"

static uint8_t led_gpios[] = {LED_PIN_1, LED_PIN_2, LED_PIN_3, LED_PIN_4};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint8_t);

// DEVICE configuration
static keyscan_gpios_t matrix = {
    .direct_len = 2,
    .direct = {SW_PIN_1, SW_PIN_3},
};

static keyscan_gpios_t matrix2 = {
    .direct_len = 2,
    .direct = {SW_PIN_2, SW_PIN_4},
};

// DONGLE configuration
static keymap_layout_t layout = {
    {LD0(0, 0), LD1(0, 0)},
    {LD0(0, 1), LD1(0, 1)},
};

static const keymap_actions_t keymap = {
    // Layer 0
    {
        {KC_A, KC_S},
        {LT(1, KC_D), MOD_LSFT},
    },

    // Layer 1
    {
        {KC_F, M(MOD_LSFT, KC_1)},
        {_______, KC_E},
    },
};

static const uint8_t rows_len = 2;
static const uint8_t cols_len = 2;
static const uint8_t layers_len = sizeof(keymap) / sizeof(keymap[0]);
//  END configuration

int example_keymap_hid() {
  init_usbd();
  init_leds(led_gpios, led_gpios_len);

  // TODO: move this into keymap initialization
  init_timer(TIMER2);

  init_keyscan_direct(0, &matrix);
  init_keyscan_direct(1, &matrix2);

  keymap_register_config(0, rows_len, cols_len, layers_len, &layout, &keymap);

  leds_blink();
  leds_set(1, 1);

  while (1) {
    __asm__("WFI");
  }
}

void KEYSCAN_EventHandler(uint8_t matrix_id, keyscan_state_t state) {
  keymap_update_state(0, 0, matrix_id, &state);

  uint32_t matrix = state.matrix[0] | (state.matrix[1] << cols_len);
  leds_set_all(matrix);
}

void KEYMAP_ReportHandler(hid_report_keyboard_t report) { hid_send_kb_report(&report); }
#ifdef EXAMPLE_KEYMAP_HID
#endif

