#ifndef UTILS_H

#include <stddef.h>
#include <stdint.h>

volatile void* memcpy(volatile void* dest, volatile void* src, size_t n);
void delay(volatile uint32_t duration);
size_t min(size_t a, size_t b);
uint8_t divide_ceil(uint8_t a, uint8_t b);

#define UTILS_H
#endif  // UTILS_H
