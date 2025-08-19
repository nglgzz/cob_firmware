// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

#include <stddef.h>
#include <stdint.h>

void init_leds(uint8_t pins[], size_t pins_size);
void leds_set(int index, int state);
void leds_set_all(uint32_t leds);
void leds_blink(uint32_t duration);

#define MAX_LED_PINS_SIZE 8

#endif  // LED_H
