// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

// stddef is defining size_t, while stdint is defining fixed width integer
// types, both are used in the "leds.c" file.
#include <stddef.h>
#include <stdint.h>

void delay(volatile uint32_t duration);
void init_leds();
void toggle_led(int index, int state);
void blink_leds();

// All of these pins are on port 0
#define LED_PIN_1 13
#define LED_PIN_2 14
#define LED_PIN_3 15
#define LED_PIN_4 16

#endif  // LED_H
