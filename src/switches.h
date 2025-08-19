#ifndef SWITCH_H
#define SWITCH_H

#include <stddef.h>
#include <stdint.h>

void init_switches(uint8_t pins[], size_t pins_size);

#define MAX_SWITCH_PINS_SIZE 32

#endif  // SWITCH_H
