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

uint8_t keyscan_gpios[MAX_GPIOS];
size_t keyscan_gpios_len;
keyscan_t keyscan_state = {};

void KEYSCAN_noop(keyscan_t keyscan) {}
void KEYSCAN_EventHandler(keyscan_t keyscan) __attribute__((weak, alias("KEYSCAN_noop")));

void init_keyscan_direct(uint8_t gpios[], uint8_t gpios_len, uint8_t n_cols) {
  keyscan_gpios_len = gpios_len <= MAX_GPIOS ? gpios_len : MAX_GPIOS;

  uint8_t computed_n_rows = divide_ceil(keyscan_gpios_len, n_cols);
  keyscan_state.n_rows = computed_n_rows < MAX_ROWS ? computed_n_rows : MAX_ROWS;
  keyscan_state.n_cols = n_cols < MAX_COLS ? n_cols : MAX_COLS;

  memcpy(keyscan_gpios, gpios, keyscan_gpios_len * sizeof(gpios[0]));

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

  for (int i = 0; i < keyscan_gpios_len; i++) {
    GPIO0->DIRCLR = (GPIO_DIRCLR_PIN0_Clear << gpios[i]);
    GPIO0->PIN_CNF[gpios[i]] = sense_low;
  }

  // Clear PORT events
  GPIOTE->EVENTS_PORT = 0;

  // Enable interrupts
  GPIOTE->INTENSET |= GPIOTE_INTENSET_PORT_Msk;
}

void init_keyscan_matrix(uint8_t rows[], uint8_t rows_len, uint8_t cols[], uint8_t cols_len,
                         uint8_t diode_direction) {
  // TODO: this is a stub
}

bool debounceTimeoutStarted = false;
uint32_t previousValue = 0;

void GPIOTE_IRQHandler() {
  if (GPIOTE->EVENTS_PORT) {
    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;

    memcpy(keyscan_state.previous_rows,
           keyscan_state.rows,
           keyscan_state.n_rows * sizeof(keyscan_state.rows[0]));

    for (int i = 0; i < keyscan_gpios_len; i++) {
      uint16_t pin = keyscan_gpios[i];
      uint32_t pinValue = GPIO0->IN & (GPIO_IN_PIN0_High << pin);

      uint8_t row = i / keyscan_state.n_cols;
      uint8_t col = i % keyscan_state.n_cols;

      // The values are flipped because the switches are pulled up (i.e. 1 is
      // low and 0 is high).
      if (pinValue) {
        keyscan_state.rows[row] &= ~(1U << col);
        GPIO0->PIN_CNF[keyscan_gpios[i]] = sense_low;

      } else {
        keyscan_state.rows[row] |= 1U << col;
        GPIO0->PIN_CNF[keyscan_gpios[i]] = sense_high;
      }
    }
    // Clear potential PORT events that could have occurred during configuration.
    GPIOTE->EVENTS_PORT = 0;

    // Ignore duplicate events
    bool duplicate_scan = true;
    for (int i = 0; i < keyscan_state.n_rows; i++) {
      if (keyscan_state.rows[i] != keyscan_state.previous_rows[i]) {
        duplicate_scan = false;
        break;
      }
    }
    if (duplicate_scan) return;

    // Ignore new events during debounce time window
    if (debounceTimeoutStarted && !timer_has_timeout_expired(TIMER2)) return;

    // Start debounce timer if needed
    if (!debounceTimeoutStarted || timer_has_timeout_expired(TIMER2)) {
      timer_start_timeout(TIMER2, DEBOUNCE_DELAY_US);
    }

    KEYSCAN_EventHandler(keyscan_state);
  }
}
