# Keyscan

This module provides key scanning functionality with support for both direct
wiring and matrix layouts. It stores key states in matrix format (bitmap array)
regardless of the physical wiring to simplify keymap translation and provide a
consistent interface.

## API Reference
### Initialization

```c
void init_keyscan_direct(uint8_t gpios[], uint8_t gpios_len, uint8_t n_cols);
```

Initializes GPIO pins for a direct-wired keyboard, with keys organized logically as a matrix.

**Parameters**:

- `gpios[]`: Array of GPIO pin numbers to configure as switch inputs
- `gpios_len`: Number of pins in the array (maximum 32)
- `n_cols`: Number of columns in the logical key matrix

**Notes**:

- All pins must be on GPIO port 0
- Pins are configured with internal pull-up resistors (switches should connect to ground)
- The function enables GPIOTE interrupts and configures pins for low-level detection
- The module automatically calculates the number of rows based on total pins and specified columns
- Although pins are directly connected to switches, the state is stored in a matrix format

```c
void init_keyscan_matrix(uint8_t rows[], uint8_t rows_len, uint8_t cols[], uint8_t cols_len, uint8_t diode_direction);
```

**Note**: This function is a placeholder for future implementation of true matrix scanning.


### Callback Registration

```c
void KEYSCAN_EventHandler(keyscan_t keyscan);
```

This is a weak-linked callback function that you must implement in your
application code to handle key matrix state changes.

**Parameters**:

- `keyscan`: A structure containing the current state of the key matrix
  - `n_rows`: Number of rows in the matrix
  - `n_cols`: Number of columns in the matrix
  - `rows[]`: Array of bitmaps where each element represents a row's state
  - `previous_rows[]`: Array of bitmaps containing the previous state

**Note on Matrix Format**:
The key states are stored in a matrix format, where:
- Each element in the `rows[]` array represents one row of keys
- Within each row bitmap, each bit represents one key in that row
- The bit position corresponds to the column in the matrix
- A value of 1 indicates the key is pressed (connected to ground)
- A value of 0 indicates the key is released

**Example Implementation**:

```c
void KEYSCAN_EventHandler(keyscan_t keyscan) {
    // Check if key at row 0, column 0 is pressed
    if (keyscan.rows[0] & (1 << 0)) {
        // Handle key press
    }

    // Check if key at row 1, column 2 is pressed
    if (keyscan.rows[1] & (1 << 2)) {
        // Handle key press
    }

    // Detect key releases by comparing with previous state
    if ((keyscan.previous_rows[0] & 0x01) && !(keyscan.rows[0] & 0x01)) {
        // Key at row 0, column 0 was released
    }
}
```

## Implementation Features

- **Debounce Protection**: The module includes built-in debounce protection with
  a 500 microsecond delay between valid trigger events. This prevents multiple
  callbacks from being triggered by a single physical button press.

- **Edge Detection**: The module automatically configures each pin to detect
  both rising and falling edges by dynamically changing the sense configuration:
  when a pin is high, it's configured to detect low signals, when a pin is low,
  it's configured to detect high signals.

- **Duplicate Event Prevention**: The handler ignores consecutive events with
  identical switch states to prevent redundant processing.

- **Matrix Mapping**: Each physical switch is mapped to a row and column
  position based on its index in the input array and the configured number of
  columns.

## Usage Example

```c
#include "keyscan.h"
#include "leds.h"

static uint8_t switch_gpios[] = {1, 2, 3, 4};
static uint8_t led_gpios[] = {17, 18, 19, 20};

int main(void) {
    init_leds(led_gpios, 4);
    // Initialize as a 2x2 matrix (2 rows, 2 columns)
    init_keyscan_direct(switch_gpios, 4, 2);

    while(1) {
        // The key matrix handler will be called automatically when key states change
        __asm__("WFI");
    }
}

void KEYSCAN_EventHandler(keyscan_t keyscan) {
    // Mirror switch states from first row to LEDs
    leds_set_all(keyscan.rows[0]);

    // Perform actions based on specific key positions
    if (keyscan.rows[0] & 0x01) {
        // Key at row 0, column 0 is pressed
        play_sound();
    }

    if (keyscan.rows[1] & 0x01) {
        // Key at row 1, column 0 is pressed
        adjust_volume();
    }

    // Check for combination: keys at (0,0) and (0,1) are both pressed
    if ((keyscan.rows[0] & 0x03) == 0x03) {
        enter_config_mode();
    }
}
```

## Limitations

- Maximum of 32 switches supported
- All switches must be on GPIO port 0
- The module assumes active-low configuration (switches connect to ground)
- Debounce delay is fixed at 500 microseconds