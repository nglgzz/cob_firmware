#include "keyscan.h"

#include <stdio.h>

#include "gpio.h"
#include "tests.h"

// --------------------------------------
// KEYSCAN CONFIGURATION
// --------------------------------------

static keyscan_gpios_t matrix = {
    .direct_len = 4,
    .direct = {0, 2, 4, 6},
};

static keyscan_gpios_t matrix1 = {
    .cols_len = 2,
    .direct_len = 4,
    .direct = {10, 11, 12, 13},
};

// --------------------------------------
// KEYSCAN HANDLER
// --------------------------------------

typedef struct {
  uint8_t port;
  uint8_t pin;
  uint8_t value;
  char* message;
} test_gpio_actions_t;

typedef struct {
  uint8_t matrix_id;
  keyscan_state_t state;
} test_keyscan_t;

static test_keyscan_t scans[20] = {};
static uint8_t scans_len = 0;

void KEYSCAN_EventHandler(uint8_t matrix_id, keyscan_state_t state) {
  scans[scans_len].matrix_id = matrix_id;
  scans[scans_len].state = state;
  scans_len++;
}

int test_keyscan_direct() {
  init_keyscan_direct(0, &matrix);
  init_keyscan_direct(1, &matrix1);

  // Assume the GPIO pins we're using are pulled up.
  for (int i = 0; i < 32; i++) {
    gpio_write(0, i, 1);
    gpio_write(1, i, 1);
  }
  scans_len = 0;

  test_gpio_actions_t gpio_actions[] = {
      {.port = 0, .pin = 2, .value = 0, .message = "matrix[0] pin[0, 1] => HIGH"},
      {.port = 0, .pin = 6, .value = 0, .message = "matrix[0] pin[0, 3] => HIGH"},
      {.port = 0, .pin = 2, .value = 1, .message = "matrix[0] pin[0, 1] => LOW"},
      {.port = 0, .pin = 0, .value = 0, .message = "matrix[0] pin[0, 0] => HIGH"},
      {.port = 0, .pin = 4, .value = 0, .message = "matrix[0] pin[0, 2] => HIGH"},
      {.port = 0, .pin = 4, .value = 1, .message = "matrix[0] pin[0, 2] => LOW"},
      {.port = 0, .pin = 0, .value = 1, .message = "matrix[0] pin[0, 0] => LOW"},
      {.port = 0, .pin = 10, .value = 0, .message = "matrix[1] pin[0, 0] => HIGH"},
      {.port = 0, .pin = 6, .value = 1, .message = "matrix[0] pin[0, 3] => LOW"},
      {.port = 0, .pin = 10, .value = 1, .message = "matrix[1] pin[0, 0] => LOW"},
      {.port = 0, .pin = 11, .value = 0, .message = "matrix[1] pin[0, 1] => HIGH"},
      {.port = 0, .pin = 11, .value = 1, .message = "matrix[1] pin[0, 1] => LOW"},
      {.port = 0, .pin = 13, .value = 0, .message = "matrix[1] pin[1, 1] => HIGH"},
      {.port = 0, .pin = 12, .value = 0, .message = "matrix[1] pin[1, 0] => HIGH"},
      {.port = 0, .pin = 13, .value = 1, .message = "matrix[1] pin[1, 1] => LOW"},
      {.port = 0, .pin = 12, .value = 1, .message = "matrix[1] pin[1, 0] => LOW"},
  };
  uint8_t gpio_actions_len = sizeof(gpio_actions) / sizeof(gpio_actions[0]);
  test_keyscan_t expected_scans[] = {
      {.matrix_id = 0, .state = {.matrix = {0b0010}}},      // matrix[0].pin[0, 1] = HIGH
      {.matrix_id = 0, .state = {.matrix = {0b1010}}},      // matrix[0].pin[0, 3] = HIGH
      {.matrix_id = 0, .state = {.matrix = {0b1000}}},      // matrix[0].pin[0, 1] = LOW
      {.matrix_id = 0, .state = {.matrix = {0b1001}}},      // matrix[0].pin[0, 0] = HIGH
      {.matrix_id = 0, .state = {.matrix = {0b1101}}},      // matrix[0].pin[0, 2] = HIGH
      {.matrix_id = 0, .state = {.matrix = {0b1001}}},      // matrix[0].pin[0, 2] = LOW
      {.matrix_id = 0, .state = {.matrix = {0b1000}}},      // matrix[0].pin[0, 0] = LOW
      {.matrix_id = 1, .state = {.matrix = {0b01, 0b00}}},  // matrix[1].pin[0, 0] = HIGH
      {.matrix_id = 0, .state = {.matrix = {0b0000}}},      // matrix[0].pin[0, 3] = LOW
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b00}}},  // matrix[1].pin[0, 0] = LOW
      {.matrix_id = 1, .state = {.matrix = {0b10, 0b00}}},  // matrix[1].pin[0, 1] = HIGH
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b00}}},  // matrix[1].pin[0, 1] = LOW
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b10}}},  // matrix[1].pin[1, 1] = HIGH
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b11}}},  // matrix[1].pin[1, 0] = HIGH
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b01}}},  // matrix[1].pin[1, 1] = LOW
      {.matrix_id = 1, .state = {.matrix = {0b00, 0b00}}},  // matrix[1].pin[1, 0] = LOW
  };

  for (int i = 0; i < gpio_actions_len; i++) {
    printf("\t\t%s\n", gpio_actions[i].message);
    gpio_write(gpio_actions[i].port, gpio_actions[i].pin, gpio_actions[i].value);

    ASSERT(expected_scans[i].matrix_id == scans[i].matrix_id,
           "\tExpected scans[%d].matrix_id to equal %d. Got %d instead.\n",
           i,
           expected_scans[i].matrix_id,
           scans[i].matrix_id);

    for (int k = 0; k < MAX_ROWS; k++) {
      ASSERT(expected_scans[i].state.matrix[k] == scans[i].state.matrix[k],
             "\tExpected scans[%d].state.matrix[%d] to equal %d. Got %d instead.\n",
             i,
             k,
             expected_scans[i].state.matrix[k],
             scans[i].state.matrix[k]);
    }
  }

  return PASS;
}

int test_keyscan() {
  run_test("keyscan direct wiring", test_keyscan_direct);
  return PASS;
}
