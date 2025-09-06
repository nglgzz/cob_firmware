#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>

// Important to note that these are the max dimensions for both physical and
// logical devices (i.e. these are the same max dimensions used in the keymap
// layout).
#define MAX_ROWS 8
#define MAX_COLS 32

// TODO: increase this value once it's possible to add GPIOs from different
// ports than GPIO0.
#define MAX_GPIOS 32

#define DIODE_DIRECTION_Unset 0
#define DIODE_DIRECTION_RowToCol 1
#define DIODE_DIRECTION_ColToRow 2

typedef struct {
  uint8_t diode_direction;

  uint8_t cols_len;
  uint8_t cols[MAX_COLS];

  uint8_t rows_len;
  uint8_t rows[MAX_ROWS];

  uint8_t direct_len;
  uint8_t direct[MAX_GPIOS];
} keyscan_gpios_t;

typedef uint32_t keyscan_matrix_t[MAX_ROWS];

typedef struct {
  // Array of bitmaps representing the state of the switches in each row.
  keyscan_matrix_t matrix;
  keyscan_matrix_t previous_matrix;
} keyscan_state_t;

//  Required fields for direct connections are `direct_len`, `direct`
void init_keyscan_direct(uint8_t matrix_id, keyscan_gpios_t* config);
void init_keyscan_matrix(uint8_t matrix_id, keyscan_gpios_t* config);

void KEYSCAN_EventHandler(uint8_t matrix_id, keyscan_state_t state);

#endif  // SWITCH_H
