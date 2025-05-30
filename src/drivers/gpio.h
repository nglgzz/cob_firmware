#ifndef GPIO_H

#include <stdbool.h>
#include <stdint.h>

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

extern gpio_t *const GPIO0;
extern gpio_t *const GPIO1;

void gpio_dir_pin_output(gpio_t *port, uint8_t pin);
void gpio_out_pin(gpio_t *port, uint8_t pin, bool value);

#define GPIO_H
#endif  // GPIO_H
