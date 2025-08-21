#ifndef PROBE_H

#include <stddef.h>
#include <stdint.h>

#include "utils.h"

// TODO: move configuration of pins to userland
// These are GPIO pins used for debugging through the power profiler.
#define PP_D0 2
#define PP_D1 3
#define PP_D2 4
#define PP_D3 5
#define PP_D4 6
#define PP_D5 7
#define PP_D6 8
#define PP_D7 10

void init_probes();

void probe_pulse(uint8_t pin);
void probe_on(uint8_t pin);
void probe_off(uint8_t pin);

#define PROBE_H
#endif  // PROBE_H
