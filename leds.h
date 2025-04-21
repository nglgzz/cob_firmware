// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

#include <stdint.h>

void delay(volatile uint32_t duration);
void init_leds();
void toggle_led(int index, int state);
void blink_leds();

#endif  // LED_H