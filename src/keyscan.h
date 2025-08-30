#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>

#define MAX_ROWS 32
#define MAX_COLS 32

// TODO: increase this value once it's possible to add GPIOs from different
// ports than GPIO0.
#define MAX_GPIOS 32

typedef struct {
  uint8_t n_rows;
  uint8_t n_cols;

  // Array of bitmaps representing the state of the switches in each row.
  uint32_t rows[MAX_ROWS];
  uint32_t previous_rows[MAX_ROWS];
} keyscan_t;

void init_keyscan_direct(uint8_t gpios[], uint8_t gpios_len, uint8_t n_cols);
void init_keyscan_matrix(uint8_t rows[], uint8_t rows_len, uint8_t cols[], uint8_t cols_len,
                         uint8_t diode_direction);

void KEYSCAN_EventHandler(keyscan_t keyscan);

#endif  // SWITCH_H
