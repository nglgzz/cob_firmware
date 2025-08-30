#include "utils.h"

volatile void* memcpy(volatile void* dest, volatile void* src, size_t n) {
  char* d = (char*)dest;
  char* s = (char*)src;

  // Copy byte by byte
  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

void delay(volatile uint32_t duration) { while (duration--); }

inline size_t min(size_t a, size_t b) { return a < b ? a : b; }

inline uint8_t divide_ceil(uint8_t a, uint8_t b) { return a % b == 0 ? (a / b) : (a / b) + 1; }
