// pragma once has the same purpose, but is not supported by all compilers, so
// ifndef is the safer choice for cross-platform compatibility.
#ifndef LED_H
#define LED_H

void init_leds();
void blink_leds();

#endif  // LED_H