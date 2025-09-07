#include "keymap.h"

#include <stdbool.h>
#include <stdint.h>

#include "keyscan.h"
#include "usb_hid.h"

static keymap_config_t configs[8] = {};
static keymap_state_t state[8] = {};

void keymap_register_config(uint8_t config_id, uint8_t rows_len, uint8_t cols_len,
                            uint8_t layers_len, const keymap_layout_t* layout,
                            const keymap_keymap_t* keymap) {
  keymap_config_t* _config = &configs[config_id];
  _config->rows_len = rows_len;
  _config->cols_len = cols_len;
  _config->layers_len = layers_len;
  _config->layout = layout;
  _config->keymap = keymap;
}

uint16_t get_keycode(uint8_t config_id, uint8_t row, uint8_t col) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];

  uint16_t keycode = 0x0;
  uint16_t current_layer = _config->layers_len;

  while (!keycode) {
    if (current_layer != 0 && (_state->active_layers & (1 << current_layer)) == 0) {
      // This layer is not active, go to lower layer.
      current_layer--;
      continue;
    }

    keycode = (*_config->keymap)[current_layer][row][col];
    if (current_layer == 0) break;

    current_layer--;
  }

  return keycode;
}

/**
 * Types of key presses:
 *
 * - KC - add exact value to report
 * - TRNS - value is picked from layer below, if already at layer 0 value is
 *   ignored
 * - Layer Tap
 *      - Keypress activate layer
 *      - On release
 *          - Deactivate layer
 *          - KC (tap) if no other keys have been pressed
 *          - ignore if keypress is longer than HOLD_DELAY
 * - MOD - toggle bit in MOD bitmap
 * - Mod Tap
 *      - Activate MOD if there are other keys being pressed
 *      - Activate MOD if keypress is longer than HOLD_DELAY
 *      - On release, KC (tap) if no other keys have been pressed
 * - MOD_KC - (tap) keycode + modifier (?)
 */
hid_report_keyboard_t keymap_update_state(uint8_t config_id, uint8_t device_id,
                                          uint8_t matrix_id, keyscan_state_t* keyscan) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];

  // Move current state to previous state
  for (int i = 0; i < _config->rows_len; i++) {
    _state->layout_previous_state[i] = _state->layout_state[i];
  }

  // Update logical state from physical state.
  //
  // The logical state is the complete state of the layout, coming from all
  // configured devices and matrixes; while the physical state is the output of a
  // keyscan for a specific device and matrix.
  for (int row = 0; row < _config->rows_len; row++) {
    for (int col = 0; col < _config->cols_len; col++) {
      keymap_layout_key_t* key = (keymap_layout_key_t*)&((*_config->layout)[row][col]);

      if (key->device_id != device_id || key->matrix_id != matrix_id) {
        continue;
      }

      bool is_key_pressed = keyscan->matrix[key->row] & (1 << key->col);
      if (is_key_pressed) {
        _state->layout_state[row] |= 1 << col;
      } else {
        _state->layout_state[row] &= ~(1 << col);
      }
    }
  }

  hid_report_keyboard_t report = {
      .report_id = 1,
      .modifiers = 0,
      ._reserved = 0,
      .keys = {0},
  };
  uint8_t report_keys_index = 0;
  uint8_t n_keys_pressed = 0;

  // Compute actions
  for (int row = 0; row < _config->rows_len; row++) {
    for (int col = 0; col < _config->cols_len; col++) {
      bool was_key_pressed = state->layout_previous_state[row] & (1 << col);
      bool is_key_pressed = state->layout_state[row] & (1 << col);

      if (is_key_pressed) {
        n_keys_pressed++;
        uint16_t keycode = get_keycode(config_id, row, col);
        if (keycode == 0) continue;

        if ((keycode >> 8) == 0) {
          // KC
          report.keys[report_keys_index] = (uint8_t)(keycode & 0xFF);
          report_keys_index = (report_keys_index + 1) % 5;
        } else if ((keycode >> 8) == 1) {
          // Modifier
          report.modifiers |= (uint8_t)(keycode & 0xFF);
        }
      }

      if (was_key_pressed && is_key_pressed) {
        // still pressed
      }

      if (was_key_pressed && !is_key_pressed) {
        // key released
      }
    }
  }

  return report;
};
