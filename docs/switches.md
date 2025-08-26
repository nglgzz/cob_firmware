# Switches


This module configures GPIO pins as inputs with pull-up resistors and interrupt
capabilities, allowing your application to respond to switch state changes
through a callback mechanism.

## API Reference
### Initialization

```c
void init_switches(uint8_t pins[], size_t pins_size);
```
Initializes the specified GPIO pins as switch inputs with pull-up resistors and
interrupt detection.

**Parameters**:

- `pins[]`: Array of GPIO pin numbers to configure as switch inputs
- `pins_size`: Number of pins in the array (maximum 32)

**Notes**:

- All pins must be on GPIO port 0
- Pins are configured with internal pull-up resistors (switches should connect
  to ground)
- The function enables GPIOTE interrupts and configures pins for low-level
  detection


### Callback Registration

```c
void SWITCHES_ToggleHandler(uint32_t switches);
```

This is a weak-linked callback function that you must implement in your
application code to handle switch state changes.

Parameters:

- `switches`: A bit field where each bit represents the state of a switch
    - `Bit 0` corresponds to the first pin in your initialization array, `Bit 1`
      corresponds to the second pin, and so on.
    - A value of 1 indicates the switch is pressed (connected to ground), while
      a value of 0 indicates the switch is released.

Example Implementation:

```c
void SWITCHES_ToggleHandler(uint32_t switches) {
    // Check if first switch is pressed
    if (switches & 0x01) {
        // Handle first switch press
    }

    // Check if second switch is pressed
    if (switches & 0x02) {
        // Handle second switch press
    }

    // Set LEDs based on switch states
    leds_set_all(switches);
}
```

## Features

- Debounce protection: The module includes built-in debounce protection with a
  500 microsecond delay between valid trigger events. This prevents multiple
  callbacks from being triggered by a single physical button press.

- Edge detection: The module automatically configures each pin to detect both
  rising and falling edges by dynamically changing the sense configuration: when
  a pin is high, it's configured to detect low signals, when a pin is low, it's
  configured to detect high signals.

- Duplicate event prevention: The handler ignores consecutive events with
  identical switch states to prevent redundant processing.

## Usage Example

```c
#include "switches.h"
#include "leds.h"

// Define switch and LED pins
static uint8_t switch_pins[] = {1, 2, 3, 4};  // GPIO pins connected to switches
static uint8_t led_pins[] = {17, 18, 19, 20}; // GPIO pins connected to LEDs

int main(void) {
    // Initialize LEDs and switches
    init_leds(led_pins, 4);
    init_switches(switch_pins, 4);

    // Main loop
    while(1) {
        // The switch handler will be called automatically when switches change state
        __WFI(); // Wait for interrupt (low power mode)
    }
}

// Implement the switch handler callback
void SWITCHES_ToggleHandler(uint32_t switches) {
    // Mirror switch states to LEDs
    leds_set_all(switches);

    // Perform actions based on specific switches
    if (switches & 0x01) {
        // First switch is pressed
        play_sound();
    }

    if ((switches & 0x03) == 0x03) {
        // Both first and second switches are pressed
        enter_config_mode();
    }
}
```

## Limitations

- Maximum of 32 switches supported
- All switches must be on GPIO port 0
- The module assumes active-low configuration (switches connect to ground)
- Debounce delay is fixed at 500 microseconds