#include "gpio.h"

#include "nrf52840_bitfields.h"

// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpio.html
#define GPIO0_BASE 0x50000000U
#define GPIO1_BASE 0x50000300U

typedef struct {
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
  volatile uint32_t __RESERVED[118];
  // (0x700) Configuration of GPIO pins
  volatile uint32_t PIN_CNF[32];
} gpio_t;

// This is defined as a variable instead of a macro for easier debugging (macros
// are strings replacements, while variables generate symbols that are usable in
// debug mode).
gpio_t *const GPIO0 = (gpio_t *)(GPIO0_BASE + 0x504U);
gpio_t *const GPIO1 = (gpio_t *)(GPIO1_BASE + 0x504U);

gpio_t *ports[] = {GPIO0, GPIO1};

void gpio_mode(uint8_t port, uint8_t pin, bool mode) {
  if (mode) {
    ports[port]->DIRSET = (GPIO_DIRSET_PIN0_Set << pin);
  } else {
    ports[port]->DIRCLR = (GPIO_DIRCLR_PIN0_Clear << pin);
  }
}

void gpio_write(uint8_t port, uint8_t pin, bool value) {
  if (value) {
    ports[port]->OUTSET = (GPIO_OUTSET_PIN0_Set << pin);
  } else {
    ports[port]->OUTCLR = (GPIO_OUTCLR_PIN0_Clear << pin);
  }
}

void gpio_set_cnf(uint8_t port, uint8_t pin, uint32_t conf) {
  ports[port]->PIN_CNF[pin] = conf;
}

uint8_t gpio_read(uint8_t port, uint8_t pin) { return (ports[port]->IN >> pin) & 0x1; }
