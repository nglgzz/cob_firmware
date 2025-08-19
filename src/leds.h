// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

#include <stddef.h>
#include <stdint.h>

void init_leds(uint8_t pins[], size_t pins_size);
void toggle_led(int index, int state);
void blink_leds();
void toggle_many(uint32_t leds);

#define MAX_LED_PINS_SIZE 8

#endif  // LED_H
