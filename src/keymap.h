#ifndef KEYMAP_H
#define KEYMAP_H

#include <stdint.h>

#include "keyscan.h"
#include "usb_hid.h"

// Struct used to read elements in the keymap_layout_t matrix.
typedef struct __attribute__((packed)) {
  uint8_t col;
  uint8_t row;
  uint8_t matrix_id;
  uint8_t device_id;
} keymap_layout_key_t;

// Array representing the translation between the indexes of the physical
// switches and the logical keymap indexes. The reason this is here and not in
// the keyscan layer, is that one layout could be composed of multiple devices /
// matrixes.
//
// An element in this array has the following bit structure:
//
// ```
// Bit number | ID
//  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
//               D  D  D  D              C  C  C  C  B  B  B  B  B  B B B A A A A A A A A
//
// A: Column index
// B: Row index
// C: Matrix ID (one device can have multiple matrix configs)
// D: Device ID
// ```
typedef const uint32_t keymap_layout_t[MAX_ROWS][MAX_COLS];

// Array that represents the mapping between the layout indexes and their
// corresponding key actions. In this array, each element represents a layer,
// and each layer is also an array, with each element being a 16 bit keycode
// representing an action.
//
// ```
// Bit number | 15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
// ID         |  C  C  C  C  B  B B B A A A A A A A A
//
// A: HID keycode
// B: Modifier / Layer (depending on the action type)
// C: Action type
//          0 = keycode
//          1 = modifier
//          2 = layer tap
//          3 = mod tap
//          4 = mod + keycode
// ```
typedef const uint16_t keymap_actions_t[][MAX_ROWS][MAX_COLS];

typedef struct {
  uint8_t rows_len;
  uint8_t cols_len;
  uint8_t layers_len;

  const keymap_layout_t* layout;
  const keymap_actions_t* actions;
} keymap_config_t;

typedef struct {
  // Bitmap of active layers, bit 0 is ignored, as layer 0 (default layer) is
  // always active.
  uint16_t active_layers;

  // bits 0-7: col
  // bits 8-15: row
  // bit 16: is_tapping
  uint32_t is_tapping;

  // Matrix representing the logical state, for each key, the first 4 bits
  // represent the layer in which the key was pressed, and the 5th bit
  // represents whether the key is currently pressed.
  uint8_t layout_state[MAX_ROWS][MAX_COLS];

  // Previous physical state, used to detect key releases.
  uint8_t layout_previous_state[MAX_ROWS][MAX_COLS];
} keymap_state_t;

void keymap_register_config(uint8_t config_id, uint8_t rows_len, uint8_t cols_len,
                            uint8_t layers_len, const keymap_layout_t* layout,
                            const keymap_actions_t* actions);

// Config ID is the config that the change should be applied to. The rest is
// information about the source event.
hid_report_keyboard_t keymap_update_state(uint8_t config_id, uint8_t device_id,
                                          uint8_t matrix_id, keyscan_state_t* keyscan);

// ----------------
// LAYOUT
// ----------------
// Layout key
#define L(device, matrix, row, col)                                                      \
  (((device << 24) & 0xFF000000) | ((matrix << 16) & 0xFF0000) | ((row << 8) & 0xFF00) | \
   (col & 0xFF))

// Layout key, assumes device 0, matrix 0
#define LD0(row, col) L(0, 0, row, col)

// Layout key, assumes device 0, matrix 1
#define LD1(row, col) L(0, 1, row, col)

// ----------------
// ACTIONS
// ----------------

#define LT(layer, keycode) (0x2 << 12 | (layer & 0xF) << 8 | (keycode & 0xFF))
#define MT(modifier, keycode) (0x3 << 12 | ((modifier) & 0xF) << 8 | (keycode & 0xFF))
#define M(modifier, keycode) (0x4 << 12 | ((modifier) & 0xF) << 8 | (keycode & 0xFF))

#define MOD_LCTRL 0x1001
#define MOD_LSFT 0x1002
#define MOD_LALT 0x1004
#define MOD_LGUI 0x1008

#define MOD_RCTRL 0x1010
#define MOD_RSFT 0x1020
#define MOD_RALT 0x1040
#define MOD_RGUI 0x1080

#define KC_TRNS 0x00
#define _______ 0x00

#define KC_A 0x04
#define KC_B 0x05
#define KC_C 0x06
#define KC_D 0x07
#define KC_E 0x08
#define KC_F 0x09
#define KC_G 0x0A
#define KC_H 0x0B
#define KC_I 0x0C
#define KC_J 0x0D
#define KC_K 0x0E
#define KC_L 0x0F
#define KC_M 0x10
#define KC_N 0x11
#define KC_O 0x12
#define KC_P 0x13
#define KC_Q 0x14
#define KC_R 0x15
#define KC_S 0x16
#define KC_T 0x17
#define KC_U 0x18
#define KC_V 0x19
#define KC_W 0x1A
#define KC_X 0x1B
#define KC_Y 0x1C
#define KC_Z 0x1D

#define KC_1 0x1E
#define KC_2 0x1F
#define KC_3 0x20
#define KC_4 0x21
#define KC_5 0x22
#define KC_6 0x23
#define KC_7 0x24
#define KC_8 0x25
#define KC_9 0x26
#define KC_0 0x27

#define KC_ENTER 0x28
#define KC_ENT 0x28
#define KC_ESCAPE 0x29
#define KC_ESC 0x29
#define KC_BACKSPACE 0x2A
#define KC_BSPC 0x2A
#define KC_TAB 0x2B
#define KC_SPACE 0x2C
#define KC_SPC 0x2C
#define KC_MINUS 0x2D
#define KC_MINS 0x2D
#define KC_EQUAL
#define KC_EQL
#define KC_LEFT_BRACKET
#define KC_LBRC
#define KC_RIGHT_BRACKET
#define KC_RBRC
#define KC_BACKSLASH 0x31
#define KC_BSLS 0x31
#define KC_NONUS_HASH 0x32
#define KC_NUHS 0x32
#define KC_SEMICOLON 0x33
#define KC_SEMI 0x33
#define KC_QUOTE 0x34
#define KC_QUOT 0x34
#define KC_GRAVE 0x35
#define KC_GRV 0x35
#define KC_COMMA 0x36
#define KC_COMM 0x36
#define KC_DOT 0x37
#define KC_SLASH 0x38
#define KC_SLSH 0x38
#define KC_CAPS_LOCK 0x39
#define KC_CAPS 0x39

#define KC_F1 0x3A
#define KC_F2 0x3B
#define KC_F3 0x3C
#define KC_F4 0x3D
#define KC_F5 0x3E
#define KC_F6 0x3F
#define KC_F7 0x40
#define KC_F8 0x41
#define KC_F9 0x42
#define KC_F10 0x43
#define KC_F11 0x44
#define KC_F12 0x45

#define KC_PRINT_SCREEN 0x46
#define KC_PSCR 0x46
#define KC_SCROL_LOCK 0x47
#define KC_SCRL 0x47
#define KC_PAUSE 0x48
#define KC_PAUS 0x48
#define KC_INSERT 0x49
#define KC_INS 0x49
#define KC_HOME 0x4A
#define KC_PAGE_UP 0x4B
#define KC_PGUP 0x4B
#define KC_DELETE 0x4C
#define KC_DEL 0x4C
#define KC_END 0x4D
#define KC_PAGE_DOWN 0x4E
#define KC_PGDN 0x4E
#define KC_RIGHT 0x4F
#define KC_RGHT 0x4F
#define KC_LEFT 0x50
#define KC_DOWN 0x51
#define KC_UP 0x52
#define KC_NUM_LOCK 0x53
#define KC_NUM 0x53

#define KC_KP_SLASH 0x54
#define KC_PSLS 0x54
#define KC_KP_ASTERISK 0x55
#define KC_PAST 0x55
#define KC_KP_MINUS 0x56
#define KC_PMNS 0x56
#define KC_KP_PLUS 0x57
#define KC_PPLS 0x57
#define KC_KP_ENTER 0x58
#define KC_PENT 0x58
#define KC_KP_1 0x59
#define KC_P1 0x59
#define KC_KP_2 0x5A
#define KC_P2 0x5A
#define KC_KP_3 0x5B
#define KC_P3 0x5B
#define KC_KP_4 0x5C
#define KC_P4 0x5C
#define KC_KP_5 0x5D
#define KC_P5 0x5D
#define KC_KP_6 0x5E
#define KC_P6 0x5E
#define KC_KP_7 0x5F
#define KC_P7 0x5F
#define KC_KP_8 0x60
#define KC_P8 0x60
#define KC_KP_9 0x61
#define KC_P9 0x61
#define KC_KP_0 0x62
#define KC_P0 0x62

#endif  // KEYMAP_H
