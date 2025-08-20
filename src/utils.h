#ifndef UTILS_H

#include <stddef.h>
#include <stdint.h>

void* memcpy(void* dest, const void* src, size_t n);
void delay(volatile uint32_t duration);
size_t min(size_t a, size_t b);

#define UTILS_H
#endif  // UTILS_H
