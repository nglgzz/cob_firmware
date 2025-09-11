#include "keymap.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "keyscan.h"
#include "tests.h"
#include "usb_hid.h"

// --------------------------------------
// KEYMAP CONFIGURATION
// --------------------------------------

static keymap_layout_t layout = {
    {LD0(0, 1), LD0(0, 0)},
    {LD0(1, 1), LD0(1, 0)},
};

static const keymap_actions_t actions = {
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
static const uint8_t layers_len = sizeof(actions) / sizeof(actions[0]);

// --------------------------------------
// REPORTS HANDLER
// --------------------------------------

static hid_report_keyboard_t reports[20] = {};
static uint8_t reports_len = 0;

void KEYMAP_ReportHandler(hid_report_keyboard_t report) {
  reports[reports_len] = report;
  reports_len++;
}

typedef struct {
  uint8_t config_id;
  uint8_t device_id;
  uint8_t matrix_id;
  keyscan_matrix_t matrix;
} test_keyscan_t;

// --------------------------------------
// TEST RUNNER
// --------------------------------------

int _keymap_run_test(uint8_t keyscans_len, test_keyscan_t keyscans[keyscans_len],
                     uint8_t expected_reports_len,
                     hid_report_keyboard_t expected_reports[expected_reports_len]) {
  keymap_register_config(0, rows_len, cols_len, layers_len, &layout, &actions);
  for (int i = 0; i < keyscans_len; i++) {
    keymap_update_state(keyscans[i].config_id,
                        keyscans[i].device_id,
                        keyscans[i].matrix_id,
                        keyscans[i].matrix);
  }
  usleep(300e3);

  ASSERT(expected_reports_len == reports_len,
         "\tExpected reports length of %d.\n\tFound %d instead.\n",
         expected_reports_len,
         reports_len);

  for (int i = 0; i < reports_len; i++) {
    for (int k = 0; k < 5; k++) {
      ASSERT(expected_reports[i].keys[k] == reports[i].keys[k],
             "\tExpected reports[%d].keys[%d] to equal %d.\n\tFound %d instead.\n",
             i,
             k,
             expected_reports[i].keys[k],
             reports[i].keys[k]);
    }

    ASSERT(expected_reports[i].modifiers == reports[i].modifiers,
           "\tExpected modifiers to equal %d.\n\tFound %d instead.\n",
           expected_reports[i].modifiers,
           reports[i].modifiers);
  }

  // Cleanup actual reports
  for (int i = 0; i < reports_len; i++) {
    hid_report_keyboard_t rep = {};
    reports[i] = rep;
  }

  return PASS;
}

// --------------------------------------
// TESTS
// --------------------------------------

// Tests keypress, mod + keycode, layer tap
int test_keymap_single_device() {
  test_keyscan_t keyscans[] = {
      {
          // Press S
          .matrix = {0b01, 0b00},
      },
      {
          // Release S
          .matrix = {0b00, 0b00},
      },
      {
          // Go to layer 1
          .matrix = {0b00, 0b10},
      },
      {
          // Press shift + 1
          .matrix = {0b01, 0b10},
      },
      {
          // Release shift + 1
          .matrix = {0b00, 0b00},
      },
      {
          // Press layer key
          .matrix = {0b00, 0b10},
      },
      {
          // Release layer key
          .matrix = {0b00, 0b00},
      },
  };
  uint8_t keyscans_len = sizeof(keyscans) / sizeof(keyscans[0]);

  hid_report_keyboard_t expected_reports[] = {
      {.keys = {KC_S}},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_D}},
      {.keys = {}},
  };
  uint8_t expected_reports_len = sizeof(expected_reports) / sizeof(expected_reports[0]);

  return _keymap_run_test(keyscans_len, keyscans, expected_reports_len, expected_reports);
}

int test_keymap() {
  run_test("Keymap single device (KC, LT, M + KC)", test_keymap_single_device);

  return PASS;
}

