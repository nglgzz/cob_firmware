#ifndef GPIO_H

#include <stdint.h>

// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpio.html
#define GPIO0_BASE 0x50000000U
#define GPIO_PIN_CNF_OFFSET 0x700U
#define GPIO_PIN_CNF_SIZE 4U

struct gpio {
  //  (0x504) GPIO output register (1 sets pin to high, 0 to low)
  volatile uint32_t OUT;
  //  (0x508) GPIO output set register: set individual bits in GPIO port (1 sets
  //  pin to high, 0 has no effect)
  volatile uint32_t OUTSET;
  //  (0x50C) GPIO output clear register: clear individual bits in GPIO port (1
  //  sets pin to low, 0 has no effect)
  volatile uint32_t OUTCLR;
  // (0x510) Read GPIO port
  volatile uint32_t IN;
  // (0x514) Direction of GPIO pins
  volatile uint32_t DIR;
  // (0x518) DIR set register
  volatile uint32_t DIRSET;
  // (0x51C) DIR clear register
  volatile uint32_t DIRCLR;
  // (0x520) Latch register indicating what GPIO pins that have met the criteria
  // set in the PIN_CNF[n].SENSE registers. When reading 1 = criteria has been
  // met, 0 = criteria has not been met. When writing 1 = clear latch
  volatile uint32_t LATCH;
  // (0x524) Select between the default DETECT signal behavior and LDETECT mode
  volatile uint32_t DETECTMODE;
};

extern struct gpio *GPIO0;

// ---------------------
// GPIO_CNF[n]
// ---------------------
enum {
  GPIO_CNF_DIR_INPUT,  // 0: Configure pin as input pin
  GPIO_CNF_DIR_OUTPUT  // 1: Configure pin as output pin
};
enum {
  GPIO_CNF_INPUT_CONNECT,    // 0: Connect input buffer
  GPIO_CNF_INPUT_DISCONNECT  // 1: Disconnect input buffer
};
enum {
  GPIO_CNF_PULL_DISABLED = 0,  // 0: No pull
  GPIO_CNF_PULL_PULLDOWN = 1,  // 1: Pull down on pin
  GPIO_CNF_PULL_PULLUP = 3     // 3: Pull up on pin
};
// Ignoring GPIO_CNF_DRIVE_* as I don't need it.
enum {
  GPIO_CNF_SENSE_DISABLED = 0,  // 0: Disabled
  GPIO_CNF_SENSE_HIGH = 2,      // 2: Sense for high level
  GPIO_CNF_SENSE_LOW = 3        // 3: Sense for low level
};

struct gpio_pin_cnf {
  uint8_t DIR;    // Pin direction. Same physical register as DIR register
  uint8_t INPUT;  // Connect or disconnect input buffer
  uint8_t PULL;   // Pull configuration
  uint8_t SENSE;  // Pin sensing mechanism
};

void gpio0_set_pin_cnf(uint8_t pin, struct gpio_pin_cnf *config);

#define GPIO_H
#endif  // GPIO_H
