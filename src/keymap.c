#include "keymap.h"

#include <stdint.h>

#include "usb_hid.h"

static keymap_state_t devices[8] = {};

void init_device(uint8_t device_id, keymap_state_t device) { devices[device_id] = device; }

uint16_t get_keycode(keymap_state_t* device, uint32_t key) {
  uint16_t keycode = 0x0;
  uint16_t current_layer = 15;

  while (!keycode) {
    if (current_layer != 0 && (device->active_layers & (1 << current_layer)) == 0) {
      // Skip checking the key in the layer is not active.
      current_layer--;
      continue;
    }

    keycode = device->layers[current_layer][key];
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
hid_report_keyboard_t device_update_state(uint8_t device_id, uint32_t switches) {
  // TODO figure out a guard for uninitialized devices
  // if (devices[device_id] == NULL) return;
  hid_report_keyboard_t report = {
      .report_id = 1,
      .modifiers = 0,
      ._reserved = 0,
      .keys = {0},
  };
  uint8_t keys_index = 0;

  keymap_state_t* device = &devices[device_id];

  uint32_t pressed = (~device->previous_state) & switches;
  uint32_t released = device->previous_state & (~switches);
  uint32_t still_pressed = device->previous_state & switches;

  for (int i = 0; i < device->n_switches; i++) {
    if (pressed & (0x01 << i)) {
      // Key pressed
      uint16_t keycode = get_keycode(device, i);
      if (keycode == 0) continue;

      if ((keycode >> 8) == 0) {
        // KC
        report.keys[keys_index] = (uint8_t)(keycode & 0xFF);
        keys_index = (keys_index + 1) % 5;
      } else if ((keycode >> 8) == 1) {
        report.modifiers |= (keycode & 0x00FF);
      }
    }

    if (still_pressed & (0x01 << i)) {
      // Key pressed from previous state
      uint16_t keycode = get_keycode(device, i);
      if (keycode == 0) continue;

      if ((keycode >> 8) == 0) {
        // KC
        report.keys[keys_index] = (uint8_t)(keycode & 0xFF);
        keys_index = (keys_index + 1) % 5;
      } else if ((keycode >> 8) == 1) {
        report.modifiers |= (keycode & 0x00FF);
      }
    }

    if (released & (0x01 << i)) {
      // Key released
      uint16_t keycode = get_keycode(device, i);
      if (keycode == 0) continue;

      if ((keycode >> 8) == 1) {
        report.modifiers &= ~(keycode & 0x00FF);
      }
    }
  }

  device->previous_state = switches;
  return report;
};
