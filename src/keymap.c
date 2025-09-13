#include "keymap.h"

#include <stdbool.h>
#include <stdint.h>

#include "keyscan.h"
#include "timer.h"
#include "usb_hid.h"

#define TAPPING_TERM_MS 250

static keymap_config_t configs[8] = {};
static keymap_state_t state[8] = {};

void KEYMAP_noop(hid_report_keyboard_t report) {}
void KEYMAP_ReportHandler(hid_report_keyboard_t report)
    __attribute__((weak, alias("KEYMAP_noop")));

void keymap_register_config(uint8_t config_id, uint8_t rows_len, uint8_t cols_len,
                            uint8_t layers_len, const keymap_layout_t* layout,
                            const keymap_actions_t* actions) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];
  _config->rows_len = rows_len;
  _config->cols_len = cols_len;
  _config->layers_len = layers_len;
  _config->layout = layout;
  _config->actions = actions;

  // Cleanup state in case this config_id has already been initialized.
  _state->active_layers = 1;
  _state->is_tapping = 0;

  for (int i = 0; i < MAX_ROWS; i++) {
    for (int k = 0; k < MAX_COLS; k++) {
      _state->layout_state[i][k] = 0;
      _state->layout_previous_state[i][k] = 0;
    }
  }
}

// Return the topmost active layer for a key (and with a valid action)
uint16_t get_top_layer(uint8_t config_id, uint8_t row, uint8_t col) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];

  uint16_t action = 0x0;
  uint16_t current_layer = _config->layers_len;

  while (!action) {
    if (current_layer != 0 && (_state->active_layers & (1 << current_layer)) == 0) {
      // This layer is not active, go to lower layer.
      current_layer--;
      continue;
    }

    action = (*_config->actions)[current_layer][row][col];
    if (current_layer == 0 || action) break;

    current_layer--;
  }

  return current_layer;
}

uint16_t get_action(uint8_t config_id, uint8_t row, uint8_t col) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];

  uint16_t action = 0x0;
  uint16_t current_layer = _state->layout_state[row][col] & 0xF;

  while (!action) {
    if (current_layer != 0 && (_state->active_layers & (1 << current_layer)) == 0) {
      // This layer is not active, go to lower layer.
      current_layer--;
      continue;
    }

    action = (*_config->actions)[current_layer][row][col];
    if (current_layer == 0 || action) break;

    current_layer--;
  }

  return action;
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
void keymap_update_state(uint8_t config_id, uint8_t device_id, uint8_t matrix_id,
                         keyscan_matrix_t keyscan_matrix) {
  keymap_config_t* _config = &configs[config_id];
  keymap_state_t* _state = &state[config_id];

  // Move current state to previous state
  for (int row = 0; row < _config->rows_len; row++) {
    for (int col = 0; col < _config->cols_len; col++) {
      _state->layout_previous_state[row][col] = _state->layout_state[row][col];
    }
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

      bool is_key_pressed = keyscan_matrix[key->row] & (1 << key->col);

      if (is_key_pressed) {
        if (_state->layout_previous_state[row][col] == 0) {
          // Update the state only if the key is newly pressed, otherwise keep
          // the layer/keypress information from the previous state.
          uint8_t active_layer = get_top_layer(config_id, row, col);
          _state->layout_state[row][col] = 0x10 | (active_layer & 0xF);
        }
      } else {
        _state->layout_state[row][col] = 0;
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
  bool has_tapped = false;
  uint8_t tapped_key_index = 0;

  // Compute actions
  for (int row = 0; row < _config->rows_len; row++) {
    for (int col = 0; col < _config->cols_len; col++) {
      bool was_key_pressed = _state->layout_previous_state[row][col];
      bool is_key_pressed = _state->layout_state[row][col];

      uint16_t action = get_action(config_id, row, col);
      if (action == 0) continue;
      uint8_t action_type = (action >> 12) & 0xF;

      if (is_key_pressed) {
        switch (action_type) {
          case 0:
            // Keycode
            report.keys[report_keys_index] = (action & 0xFF);
            report_keys_index = (report_keys_index + 1) % 5;
            break;
          case 1:
            // Modifier
            report.modifiers |= (action & 0xFF);
            break;

          case 2:
            // Layer tap
            _state->active_layers |= 1 << ((action >> 8) & 0xF);
            break;

          case 3:
            // Modifier tap
            report.modifiers |= ((action >> 8) & 0xF);
            break;

          case 4:
            // Modifier + keycode
            report.modifiers |= ((action >> 8) & 0xF);
            report.keys[report_keys_index] = (action & 0xFF);
            report_keys_index = (report_keys_index + 1) % 5;
            break;
        }
      }

      if (!was_key_pressed && is_key_pressed) {
        // new keypress
        switch (action_type) {
          case 2:  // Layer tap
          case 3:  // Modifier tap
            _state->is_tapping = 1 << 16 | (row & 0xFF) << 8 | (col & 0xFF);
            timer_start_timeout(timer_id_keymap, TAPPING_TERM_MS * 1000);
            break;

          default:
            _state->is_tapping = 0;
            timer_stop_timeout(timer_id_keymap);
        }
      }

      if (was_key_pressed && !is_key_pressed) {
        // key released
        switch (action_type) {
          case 2:
            // Layer tap
            _state->active_layers &= ~(1 << ((action >> 8) & 0xF));
            break;

          case 3:
            // Modifier tap
            report.modifiers &= ~((action >> 8) & 0xF);
            break;
        }

        uint8_t tapping_col = _state->is_tapping & 0xFF;
        uint8_t tapping_row = (_state->is_tapping >> 8) & 0xFF;
        bool is_tapping =
            ((_state->is_tapping >> 16) & 0x01) && tapping_col == col && tapping_row == row;
        if (is_tapping && !timer_has_timeout_expired(timer_id_keymap)) {
          // Used to send keyrelease report
          has_tapped = true;
          tapped_key_index = report_keys_index;

          report.keys[report_keys_index] = (action & 0xFF);
          report_keys_index = (report_keys_index + 1) % 5;
        }

        _state->is_tapping = 0;
        timer_stop_timeout(timer_id_keymap);
      }
    }
  }

  KEYMAP_ReportHandler(report);

  if (has_tapped) {
    timer_sleep_ms(10);
    report.keys[tapped_key_index] = 0;
    KEYMAP_ReportHandler(report);
  }
};
