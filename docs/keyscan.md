# Keyscan

This module provides key scanning functionality with support for both direct
wiring and matrix layouts. It stores key states in matrix format (bitmap array)
regardless of the physical wiring to simplify keymap translation and provide a
consistent interface.

The module supports multiple keyscan configurations, allowing hybrid devices
that use both direct wiring and matrix layouts in different sections of the same
device.

- [API Reference](#api-reference)
  - [Data Structures](#data-structures)
  - [Initialization](#initialization)
  - [Callback Registration](#callback-registration)
- [Multiple Configuration Support](#multiple-configuration-support)
- [Implementation Features](#implementation-features)
- [Usage Example](#usage-example)
- [Limitations](#limitations)


## API Reference
### Data Structures

```c
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
  // Array of bitmaps representing the state of the switches in each row
  uint32_t rows[MAX_ROWS];
  uint32_t previous_rows[MAX_ROWS];
} keyscan_state_t;
```


### Initialization

```c
void init_keyscan_direct(uint8_t config_id, keyscan_config_t* config);
```

Initializes GPIO pins for a direct-wired key matrix with the specified configuration ID.

**Parameters**:

- `config_id`: ID for this configuration (0-3)
- `config`: Pointer to configuration structure with the following fields:
  - `gpios[]`: Array of GPIO pin numbers to configure as switch inputs
  - `gpios_len`: Number of pins in the array (maximum 32)
  - `cols_len`: Number of columns in the logical key matrix (optional, defaults to MAX_COLS)
  - `rows_len`: Number of rows in the logical key matrix (optional, calculated from gpios_len/cols_len)

**Notes**:

- All pins must be on GPIO port 0
- The module supports up to 4 different configurations (config_id 0-3)
- Pins are configured with internal pull-up resistors (switches should connect to ground)
- The function enables GPIOTE interrupts and configures pins for low-level detection
- If not specified, the module automatically calculates the number of rows based on total pins and specified columns

```c
void init_keyscan_matrix(uint8_t config_id, keyscan_config_t* config);
```

**Note**: This function is a placeholder for future implementation of true matrix scanning.


### Callback Registration

```c
void KEYSCAN_EventHandler(uint8_t config_id, keyscan_state_t state);
```

This is a weak-linked callback function that you must implement in your
application code to handle key matrix state changes.

**Parameters**:

- `config_id`: ID of the configuration that triggered the event (0-3)
- `state`: A structure containing the current state of the key matrix
  - `rows[]`: Array of bitmaps where each element represents a row's state
  - `previous_rows[]`: Array of bitmaps containing the previous state

**Note on Matrix Format**: The key states are stored in a matrix format, where:

- Each element in the rows[] array represents one row of keys
- Within each row bitmap, each bit represents one key in that row
- The bit position corresponds to the column in the matrix
- A value of 1 indicates the key is pressed (connected to ground)
- A value of 0 indicates the key is released

**Example Implementation**:

```c
void KEYSCAN_EventHandler(uint8_t config_id, keyscan_state_t state) {
  // Check which configuration triggered the event
  switch (config_id) {
    case 0:  // Main keyboard matrix
      if (state.rows[0] & (1 << 0)) {
        // Handle key press
      }

      // Detect key releases by comparing with previous state
      if ((keyscan.previous_rows[0] & (1 << 0)) && !(keyscan.rows[0] & (1 << 0))) {
        // Handle key release
      }
      break;

    case 1:  // Modifiers (direct wired)
      // Check if modifier at row 0, column 2 is pressed
      if (state.rows[0] & (1 << 2)) {
        // Handle modifier press
      }
      break;
  }
}

```

## Multiple Configuration Support

The module supports up to 4 different keyscan configurations (with IDs 0-3),
allowing for hybrid device designs where a combination of direct wiring and
matrix layout is used. Each configuration has its own state tracking and can
trigger the event handler independently.

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

    keyscan_config_t config = {
        .gpios = switch_gpios,
        .gpios_len = 4,
        .cols_len = 2  // 2x2 matrix
    };

    // Initialize keyscan with configuration ID 0
    init_keyscan_direct(0, &config);


    while(1) {
        // The key matrix handler will be called automatically when key states change
        __asm__("WFI");
    }
}

void KEYSCAN_EventHandler(uint8_t config_id, keyscan_state_t state) {
    // Mirror switch states from first row to LEDs
    leds_set_all(state.rows[0]);

    // Key at row 0, column 0
    if (state.rows[0] & (1 << 0)) {
        play_sound();
    }

    // Check for key combination: keys at (0,0) and (0,1) are both pressed
    if ((state.rows[0] & 0x03) == 0x03) {
        enter_config_mode();
    }

    // Detect key releases by comparing with previous state
    if ((state.previous_rows[0] & (1 << 0)) && !(state.rows[0] & (1 << 0))) {
        // Key at row 0, column 0 was released
        stop_sound();
    }
}
```

## Limitations

- Maximum of 32 switches supported
- Maximum of 4 separate configurations
- All switches must be on GPIO port 0
- The module assumes active-low configuration (switches connect to ground)
- Debounce delay is fixed at 500 microseconds