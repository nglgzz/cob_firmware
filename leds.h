// TODO: What is this for?
#ifndef LED_H
#define LED_H

// TODO: what are these libraries?
#include <stddef.h>
#include <stdint.h>

// TODO: double check these values with the datasheet
#define GPIO_BASE 0x50000000U

// GPIO output register
#define GPIO_OUT (*(volatile unsigned int *)(GPIO_BASE + 0x504))

// GPIO output set register
#define GPIO_OUTSET (*(volatile unsigned int *)(GPIO_BASE + 0x508))

// GPIO output clear register
#define GPIO_OUTCLR (*(volatile unsigned int *)(GPIO_BASE + 0x50C))

// GPIO direction register
#define GPIO_DIR (*(volatile unsigned int *)(GPIO_BASE + 0x514))

#define LED_PIN_1 13
#define LED_PIN_2 14
#define LED_PIN_3 15
#define LED_PIN_4 16

void init_leds();
void blink_leds();

#endif  // LED_H