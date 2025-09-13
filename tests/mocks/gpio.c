#include "gpio.h"

uint32_t gpios[2] = {0, 0};

void GPIOTE_noop() {}
void GPIOTE_PortEventHandler() __attribute__((weak, alias("GPIOTE_noop")));

void gpio_mode(uint8_t port, uint8_t pin, bool mode) {}

void gpio_write(uint8_t port, uint8_t pin, bool value) {
  if (value) {
    gpios[port] |= 1 << pin;
  } else {
    gpios[port] &= ~(1 << pin);
  }

  GPIOTE_PortEventHandler();
}

void gpio_set_cnf(uint8_t port, uint8_t pin, uint32_t conf) {}

uint8_t gpio_read(uint8_t port, uint8_t pin) { return (gpios[port] & (1 << pin)) != 0; }

