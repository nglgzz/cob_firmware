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

// Device 0 handles col 0, Device 1 handles col 1
static keymap_layout_t layout_multi_device = {
    {LD0(0, 0), LD1(0, 0)},
    {LD0(0, 1), LD1(0, 1)},
};

// Matrix 1 handles row 1, col 1; everything else is handled by Matrix 0
static keymap_layout_t layout_multi_matrix = {
    {LD0(0, 1), L(0, 1, 0, 1)},
    {LD0(1, 1), L(0, 1, 0, 0)},
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
  char* message;
} test_keyscan_t;

// --------------------------------------
// TEST RUNNER
// --------------------------------------

int _keymap_run_test(keymap_layout_t* layout, uint8_t keyscans_len,
                     test_keyscan_t keyscans[keyscans_len], uint8_t expected_reports_len,
                     hid_report_keyboard_t expected_reports[expected_reports_len]) {
  keymap_register_config(0, rows_len, cols_len, layers_len, layout, &actions);
  for (int i = 0; i < keyscans_len; i++) {
    printf("\t\t%s\n", keyscans[i].message);
    keymap_update_state(keyscans[i].config_id,
                        keyscans[i].device_id,
                        keyscans[i].matrix_id,
                        keyscans[i].matrix);
  }

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
  return PASS;
}

void _keymap_teardown() {
  for (int i = 0; i < reports_len; i++) {
    hid_report_keyboard_t rep = {};
    reports[i] = rep;
  }
  reports_len = 0;
}

// --------------------------------------
// TESTS
// --------------------------------------

int test_keymap_single_device() {
  test_keyscan_t keyscans[] = {
      {
          .message = "Keycode:\t ↓S (KC_S)",
          .matrix = {0b01, 0b00},
      },
      {
          .message = "Keycode:\t ↑S",
          .matrix = {0b00, 0b00},
      },
      {
          .message = "Layer tap:\t ↓LT1 (activate layer 1)",
          .matrix = {0b00, 0b10},
      },
      {
          .message = "LT, Mod+KC:\t -LT1 ↓! (KC_1 + MOD_LSFT)",
          .matrix = {0b01, 0b10},
      },
      {
          .message = "LT, Mod+KC:\t ↑LT1 -!",
          .matrix = {0b01, 0b00},
      },
      {
          .message = "LT, Mod+KC:\t ↑!",
          .matrix = {0b00, 0b00},
      },
      {
          .message = "Layer tap:\t ↓LT1",
          .matrix = {0b00, 0b10},
      },
      {
          .message = "Layer tap:\t ↑LT1 (KC_D)",
          .matrix = {0b00, 0b00},
      },
      {
          .message = "Modifier:\t ↓Shift (MOD_LSFT)",
          .matrix = {0b00, 0b01},
      },
      {
          .message = "Mod, LT:\t -Shift ↓LT1 (MOD_LSFT)",
          .matrix = {0b00, 0b11},
      },
      {
          .message = "Mod, LT, KC:\t -Shift -LT1 ↓F (MOD_LSFT + KC_F)",
          .matrix = {0b10, 0b11},
      },
      {
          .message = "Mod, LT, KC:\t -Shift -LT1 ↑F (MOD_LSFT)",
          .matrix = {0b00, 0b11},
      },
      {
          .message = "Mod, LT:\t ↑Shift -LT1",
          .matrix = {0b00, 0b10},
      },
      {
          .message = "Layer tap:\t ↑LT1",
          .matrix = {0b00, 0b00},
      },
  };
  uint8_t keyscans_len = sizeof(keyscans) / sizeof(keyscans[0]);

  hid_report_keyboard_t expected_reports[] = {
      {.keys = {KC_S}},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_D}},
      {.keys = {}},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_F}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
  };
  uint8_t expected_reports_len = sizeof(expected_reports) / sizeof(expected_reports[0]);

  return _keymap_run_test(
      &layout, keyscans_len, keyscans, expected_reports_len, expected_reports);
}

int test_keymap_multi_device() {
  test_keyscan_t keyscans[] = {
      {
          .message = "Keycode:\t ↓S (KC_S)",
          .matrix = {0b01},
          .device_id = 1,
      },
      {
          .message = "Keycode:\t ↑S",
          .matrix = {0b00},
          .device_id = 1,
      },
      {
          .message = "Layer tap:\t ↓LT1 (activate layer 1)",
          .matrix = {0b10},
          .device_id = 0,
      },
      {
          .message = "LT, Mod+KC:\t -LT1 ↓! (KC_1 + MOD_LSFT)",
          .matrix = {0b01},
          .device_id = 1,
      },
      {
          .message = "Mod+KC:\t ↑LT1 -!",
          .matrix = {0b00},
          .device_id = 0,
      },
      {
          .message = "LT, Mod+KC:\t ↑!",
          .matrix = {0b00},
          .device_id = 1,
      },
      {
          .message = "Layer tap:\t ↓LT1",
          .matrix = {0b10},
          .device_id = 0,
      },
      {
          .matrix = {0b00},
          .device_id = 0,
          .message = "Layer tap:\t ↑LT1 (KC_D)",
      },
      {
          .matrix = {0b10},
          .device_id = 1,
          .message = "Modifier:\t ↓Shift (MOD_LSFT)",
      },
      {
          .matrix = {0b10},
          .device_id = 0,
          .message = "Mod, LT:\t -Shift ↓LT1 (MOD_LSFT)",
      },
      {
          .matrix = {0b11},
          .device_id = 0,
          .message = "Mod, LT, KC:\t -Shift -LT1 ↓F (MOD_LSFT + KC_F)",
      },
      {
          .matrix = {0b00},
          .device_id = 0,
          .message = "Mod, LT, KC:\t -Shift ↑LT1 ↑F",
      },
      {
          .matrix = {0b00},
          .device_id = 1,
          .message = "Mod, LT, KC:\t ↑Shift",
      },
  };
  uint8_t keyscans_len = sizeof(keyscans) / sizeof(keyscans[0]);

  hid_report_keyboard_t expected_reports[] = {
      {.keys = {KC_S}},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_D}},
      {.keys = {}},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_F}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
  };
  uint8_t expected_reports_len = sizeof(expected_reports) / sizeof(expected_reports[0]);

  return _keymap_run_test(
      &layout_multi_device, keyscans_len, keyscans, expected_reports_len, expected_reports);
}

int test_keymap_multi_matrix() {
  test_keyscan_t keyscans[] = {
      {
          .message = "Keycode:\t ↓S (KC_S)",
          .matrix_id = 1,
          .matrix = {0b10},
      },
      {
          .message = "Keycode:\t ↑S",
          .matrix_id = 1,
          .matrix = {0b00},
      },
      {
          .message = "Layer tap:\t ↓LT1 (activate layer 1)",
          .matrix = {0b00, 0b10},
      },
      {
          .message = "LT, Mod+KC:\t -LT1 ↓! (KC_1 + MOD_LSFT)",
          .matrix_id = 1,
          .matrix = {0b10},
      },
      {
          .message = "LT, Mod+KC:\t ↑LT1 -!",
          .matrix = {0b01, 0b00},
      },
      {
          .message = "LT, Mod+KC:\t ↑!",
          .matrix_id = 1,
          .matrix = {0b00},
      },
      {
          .message = "Layer tap:\t ↓LT1",
          .matrix = {0b00, 0b10},
      },
      {
          .message = "Layer tap:\t ↑LT1 (KC_D)",
          .matrix = {0b00, 0b00},
      },
      {
          .message = "Modifier:\t ↓Shift (MOD_LSFT)",
          .matrix_id = 1,
          .matrix = {0b01},
      },
      {
          .message = "Mod, LT:\t -Shift ↓LT1 (MOD_LSFT)",
          .matrix = {0b00, 0b11},
      },
      {
          .message = "Mod, LT, KC:\t -Shift -LT1 ↓F (MOD_LSFT + KC_F)",
          .matrix = {0b10, 0b11},
      },
      {
          .message = "Mod, LT, KC:\t -Shift -LT1 ↑F (MOD_LSFT)",
          .matrix = {0b00, 0b11},
      },
      {
          .message = "Mod, LT:\t ↑Shift -LT1",
          .matrix_id = 1,
          .matrix = {0b00},
      },
      {
          .message = "Layer tap:\t ↑LT1",
          .matrix = {0b00, 0b00},
      },
  };
  uint8_t keyscans_len = sizeof(keyscans) / sizeof(keyscans[0]);

  hid_report_keyboard_t expected_reports[] = {
      {.keys = {KC_S}},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_1}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
      {.keys = {KC_D}},
      {.keys = {}},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {KC_F}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}, .modifiers = MOD_LSFT & 0xFF},
      {.keys = {}},
      {.keys = {}},
  };
  uint8_t expected_reports_len = sizeof(expected_reports) / sizeof(expected_reports[0]);

  return _keymap_run_test(
      &layout_multi_matrix, keyscans_len, keyscans, expected_reports_len, expected_reports);
}

int test_keymap() {
  run_test("Keymap single device", test_keymap_single_device);
  _keymap_teardown();

  run_test("Keymap multi device", test_keymap_multi_device);
  _keymap_teardown();

  run_test("Keymap multi matrix", test_keymap_multi_matrix);
  _keymap_teardown();

  return PASS;
}

