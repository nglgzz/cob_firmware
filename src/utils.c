#include "utils.h"

void* memcpy(void* dest, const void* src, size_t n) {
  char* d = (char*)dest;
  char* s = (char*)src;

  // Copy byte by byte
  while (n--) {
    *d++ = *s++;
  }

  return dest;
}

void delay(volatile uint32_t duration) { while (duration--); }
