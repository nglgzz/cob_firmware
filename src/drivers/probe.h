#ifndef PROBE_H

#include <stdbool.h>
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

typedef enum {
  probe_tag_noop,
  probe_tag_switch_handler,
  probe_tag_radio_send,
  probe_tag_radio_tx,
  probe_tag_radio_receive,
  probe_tag_radio_rx,
  probe_tag_radio_status,
  probe_tag_radio_payload,
  probe_tag_timeout,
} probe_tag_t;

void init_probes(probe_tag_t tags[], size_t size);
void probe_on(probe_tag_t tag);
void probe_off(probe_tag_t tag);
void probe_pulse(probe_tag_t tag);
void probe_pulse_times(probe_tag_t tag, uint32_t count);

extern bool probes_initialized;

#define PROBE_H
#endif  // PROBE_H
