#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>

#define MAX_ROWS 32
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

  uint8_t gpios_len;
  uint8_t gpios[MAX_GPIOS];
} keyscan_config_t;

typedef struct {
  // Array of bitmaps representing the state of the switches in each row.
  uint32_t rows[MAX_ROWS];
  uint32_t previous_rows[MAX_ROWS];
} keyscan_state_t;

//  Required fields for direct connections are `gpios`, `gpios_len`, `n_cols`
void init_keyscan_direct(uint8_t config_id, keyscan_config_t* config);
void init_keyscan_matrix(uint8_t config_id, keyscan_config_t* config);

void KEYSCAN_EventHandler(uint8_t config_id, keyscan_state_t state);

#endif  // SWITCH_H
