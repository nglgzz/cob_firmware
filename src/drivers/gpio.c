#include "gpio.h"

// This is defined as a variable instead of a macro for easier debugging (macros
// are strings replacements, while variables generate symbols that are usable in
// debug mode).
gpio_t *const GPIO0 = (gpio_t *)(GPIO0_BASE + 0x504U);
gpio_t *const GPIO1 = (gpio_t *)(GPIO1_BASE + 0x504U);

void gpio_dir_pin_output(gpio_t *port, uint8_t pin) {
  // toggling GPIO_DIR at pin number `pin` to high
  // which makes it an output pin.
  port->DIRSET = (GPIO_DIRSET_Set << pin);
}

void gpio_out_pin(gpio_t *port, uint8_t pin, bool value) {
  if (value) {
    port->OUTSET = (1 << pin);
  } else {
    port->OUTCLR = (1 << pin);
  }
}
