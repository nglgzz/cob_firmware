#include "gpio.h"

// This is defined as a variable instead of a macro for easier debugging (macros
// are strings replacements, while variables generate symbols that are usable in
// debug mode).
struct gpio *GPIO0 = (struct gpio *)(GPIO0_BASE + 0x504U);

volatile uint32_t *pin_cnf = (volatile uint32_t *)(GPIO0_BASE + GPIO_PIN_CNF_OFFSET);

/**
 * Set configuration for the specified GPIO pin in port 0.
 *
 * - A[0] DIR: 0 = input, 1 = output (same as DIR register)
 * - B[1] INPUT: 0 = connect input buffer, 1 =  disconnect input buffer
 * - C[2,3] PULL: 0 = no pull, 1 = pulldown, 3 = pullup
 * - D[8,9,10] DRIVE: ...
 * - E[16,17] SENSE: 0 = disabled, 2 = sense high level, 3 = sense low level
 */
void gpio0_set_pin_cnf(uint8_t pin, struct gpio_pin_cnf *config) {
  uint32_t cnf = (config->DIR & 0x1) |         // bit 0
                 (config->INPUT & 0x1) << 1 |  // bit 1
                 (config->PULL & 0x3) << 2 |   // bits 2,3
                 (config->SENSE & 0x3) << 16;  // bits 16,17

  pin_cnf[pin] = cnf;
}
