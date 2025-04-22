#ifndef GPIOTE_H

// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpiote.html
#define GPIOTE_BASE 0x40006000U

#include <stdint.h>

struct gpiote {
  // (0x17C) Event generated from multiple input GPIO pins with SENSE mechanism
  // enabled (1 = event generated, 0 = event not generated, only one bit is
  // used).
  volatile uint32_t EVENTS_PORT;

  // (0x304 - (0x17c + 4)) / 4 = 0x61 = 97
  volatile uint32_t __RESERVED[97];

  // (0x304) Enable interrupt for event (set desired pin to 1 to enable)
  volatile uint32_t INTENSET;
  // (0x308) Disable interrupt for event (set desired pin to 1 to disable)
  volatile uint32_t INTENCLR;
};

extern struct gpiote *GPIOTE;

// Bit number representing the PORT event when setting INTEN* values.
#define PORT_EVENT 31

#define GPIOTE_H
#endif  // GPIOTE_H
