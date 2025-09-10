#ifndef GPIO_H

#include <stdbool.h>
#include <stdint.h>

// Configure the direction of the specified pin, 1=output 0=input
void gpio_mode(uint8_t port, uint8_t pin, bool mode);

// Set the output value of a pin 1=high 0=low
void gpio_write(uint8_t port, uint8_t pin, bool value);

void gpio_set_cnf(uint8_t port, uint8_t pin, uint32_t conf);

uint8_t gpio_read(uint8_t port, uint8_t pin);

#define GPIO_H
#endif  // GPIO_H
