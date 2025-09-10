#include "gpiote.h"

#include "nrf52840_bitfields.h"

void GPIOTE_noop() {}
void GPIOTE_PortEventHandler() __attribute__((weak, alias("GPIOTE_noop")));

// https://docs.nordicsemi.com/bundle/ps_nrf52840/page/gpiote.html
#define GPIOTE_BASE 0x40006000U

typedef struct {
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
} gpiote_t;

gpiote_t *const GPIOTE = (gpiote_t *)(GPIOTE_BASE + 0x17CU);

void gpiote_disable_port_events() { GPIOTE->INTENCLR |= GPIOTE_INTENCLR_PORT_Msk; }

void gpiote_enable_port_events() {
  GPIOTE->EVENTS_PORT = 0;
  GPIOTE->INTENSET |= GPIOTE_INTENSET_PORT_Msk;
}

void GPIOTE_IRQHandler() {
  if (GPIOTE->EVENTS_PORT) {
    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;
    GPIOTE_PortEventHandler();
  }
}
