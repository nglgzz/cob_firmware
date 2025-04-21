#ifndef GPIO_H

#include <stdint.h>

#define GPIO0_BASE 0x50000000U
#define GPIO_PIN_CNF_OFFSET 0x700U
#define GPIO_PIN_CNF_SIZE 4U

struct gpio {
  volatile uint32_t OUT,  // (0x504) Write GPIO port
      OUTSET,             // (0x508) Set individual bits in GPIO port
      OUTCLR,             // (0x50C) Clear individual bits in GPIO port
      IN,                 // (0x510) Read GPIO port
      DIR,                // (0x514) Direction of GPIO pins
      DIRSET,             // (0x518) DIR set register
      DIRCLR,             // (0x51C) DIR clear register
      LATCH,  // (0x520) Latch register indicating what GPIO pins that have met
              // the criteria set in the PIN_CNF[n].SENSE registers
      DETECTMODE;  // (0x524) Select between default DETECT signal behavior and
                   // LDETECT mode
};

extern struct gpio *GPIO0;

enum {
  GPIO_DIR_INPUT,  // Configure pin as input pin
  GPIO_DIR_OUTPUT  // Configure pin as output pin
};
enum {
  GPIO_INPUT_CONNECT,    // Connect input buffer
  GPIO_INPUT_DISCONNECT  // Disconnect input buffer
};
enum {
  GPIO_PULL_DISABLED = 0,  // No pull
  GPIO_PULL_PULLDOWN = 1,  // Pull down on pin
  GPIO_PULL_PULLUP = 3     // Pull up on pin
};
// Ignoring GPIO_DRIVE_* as I don't need it.
enum {
  GPIO_SENSE_DISABLED = 0,
  GPIO_SENSE_HIGH = 2,  // Sense for high level
  GPIO_SENSE_LOW = 3    // Sense for low level
};

struct gpio_pin_cnf {
  uint8_t DIR,  // Pin direction. Same physical register as DIR register
      INPUT,    // Connect or disconnect input buffer
      PULL,     // Pull configuration
      SENSE;    // Pin sensing mechanism
};

void gpio_set_pin_cnf(unsigned int base, uint8_t pin,
                      struct gpio_pin_cnf *config);

#define GPIO_H
#endif  // GPIO_H
