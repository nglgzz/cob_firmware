/**
 * NOTES:
 *    - Max number of switches is determined by MAX_GPIOS.
 *    - All switches need to be on GPIO port 0.
 */
#include "keyscan.h"

#include <stdbool.h>
#include <stddef.h>

#include "core.h"
#include "gpio.h"
#include "gpiote.h"
#include "nrf52840_bitfields.h"
#include "timer.h"
#include "utils.h"

#define DEBOUNCE_DELAY_US 500

static const uint32_t sense_low = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                  (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                  (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                  (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);

static const uint32_t sense_high = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos);

keyscan_gpios_t configs[4];
keyscan_state_t state[4];

void KEYSCAN_noop(uint8_t matrix_id, keyscan_state_t state) {}
void KEYSCAN_EventHandler(uint8_t matrix_id, keyscan_state_t state)
    __attribute__((weak, alias("KEYSCAN_noop")));

void init_keyscan_direct(uint8_t matrix_id, keyscan_gpios_t* config) {
  keyscan_gpios_t* _config = &configs[matrix_id];

  _config->direct_len = min(config->direct_len, MAX_GPIOS);

  if (config->cols_len == 0) {
    // If the number of columns is not specified, assume the max number of columns.
    _config->cols_len = MAX_COLS;
  } else {
    _config->cols_len = min(config->cols_len, MAX_COLS);
  }

  if (config->rows_len == 0) {
    // If the number of rows is not specified compute the rows based on the number of columns
    // and the number of GPIOs.
    uint8_t computed_n_rows = divide_ceil(_config->direct_len, _config->cols_len);
    _config->rows_len = min(computed_n_rows, MAX_ROWS);
  } else {
    _config->rows_len = min(config->rows_len, MAX_ROWS);
  }

  memcpy(_config->direct, config->direct, _config->direct_len * sizeof(_config->direct[0]));

  // Enable the GPIOTE interrupt request handler. If this is not set, the
  // peripheral can still generate interrupts, but they end up permanently
  // pending as the handlers are not executed.
  NVIC_EnableIRQ(GPIOTE_IRQn);

  /**
   * In order to prevent spurious interrupts from the PORT event while
   * configuring the sources, the following must be performed:
   *
   *   1. Disable interrupts on the PORT event (through INTENCLR.PORT).
   *   2. Configure the sources (PIN_CNF[n].SENSE).
   *   3. Clear any potential event that could have occurred during
   * configuration (write '0' to EVENTS_PORT).
   *   4. Enable interrupts (through INTENSET.PORT).
   */
  GPIOTE->INTENCLR |= GPIOTE_INTENCLR_PORT_Msk;

  for (int i = 0; i < _config->direct_len; i++) {
    GPIO0->DIRCLR = (GPIO_DIRCLR_PIN0_Clear << _config->direct[i]);
    GPIO0->PIN_CNF[_config->direct[i]] = sense_low;
  }

  // Clear PORT events
  GPIOTE->EVENTS_PORT = 0;

  // Enable interrupts
  GPIOTE->INTENSET |= GPIOTE_INTENSET_PORT_Msk;
}

void init_keyscan_matrix(uint8_t matrix_id, keyscan_gpios_t* config) {
  // TODO: this is a stub
}

bool debounceTimeoutStarted = false;
// Returns true if the debounced action should be skipped, false otherwise.
static inline bool keyscan_debounce() {
  // Ignore new events during debounce time window
  if (debounceTimeoutStarted && !timer_has_timeout_expired(TIMER2)) return true;

  // Start debounce timer if needed
  if (!debounceTimeoutStarted || timer_has_timeout_expired(TIMER2)) {
    timer_start_timeout(TIMER2, DEBOUNCE_DELAY_US);
    // debounceTimeoutStarted = true;
  }

  return false;
}

// Returns true if there is a key change, false otherwise.
static bool keyscan_direct(uint8_t matrix_id) {
  keyscan_gpios_t* _config = &configs[matrix_id];
  keyscan_state_t* _state = &state[matrix_id];

  memcpy(
      _state->previous_matrix, _state->matrix, _config->rows_len * sizeof(_state->matrix[0]));

  for (int i = 0; i < _config->direct_len; i++) {
    uint16_t gpio_pin = _config->direct[i];
    uint32_t gpio_value = GPIO0->IN & (GPIO_IN_PIN0_High << gpio_pin);

    uint8_t row = i / _config->cols_len;
    uint8_t col = i % _config->cols_len;

    // The values are flipped because the switches are pulled up (i.e. 1 is
    // low and 0 is high).
    if (gpio_value) {
      _state->matrix[row] &= ~(1U << col);
      GPIO0->PIN_CNF[gpio_pin] = sense_low;
    } else {
      _state->matrix[row] |= 1U << col;
      GPIO0->PIN_CNF[gpio_pin] = sense_high;
    }
  }

  // Ignore duplicate events
  bool duplicate_scan = true;
  for (int i = 0; i < _config->rows_len; i++) {
    if (_state->matrix[i] != _state->previous_matrix[i]) {
      duplicate_scan = false;
      break;
    }
  }
  if (duplicate_scan) return false;

  // Ignore events during debounce window
  if (keyscan_debounce()) return false;

  return true;
}

void GPIOTE_IRQHandler() {
  if (GPIOTE->EVENTS_PORT) {
    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;

    for (int i = 0; i < 4; i++) {
      if (configs[i].direct_len == 0) {
        continue;
      }

      if (keyscan_direct(i)) {
        KEYSCAN_EventHandler(i, state[i]);
      }
    }
    GPIOTE->EVENTS_PORT = 0;
  }
}
