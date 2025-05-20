#ifndef PROBE_H

#include <stddef.h>
#include <stdint.h>

#include "utils.h"

// These are GPIO pins used for debugging through the power profiler.
#define PP_D0 02
#define PP_D1 03
#define PP_D2 04
#define PP_D3 05
#define PP_D4 06
#define PP_D5 07

void init_probes();

void probe_pulse(uint8_t pin);
void probe_on(uint8_t pin);
void probe_off(uint8_t pin);

#define PROBE_H
#endif  // PROBE_H
