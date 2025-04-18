#include "switches.h"

#include "leds.h"
#include "nrf52840.h"

static uint16_t switch_pins[] = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(switch_pins[0]);

void init_switches() {
  /**
   * In order to prevent spurious interrupts from the PORT event while
   * configuring the sources, the following must be performed:
   *
   *   1. Disable interrupts on the PORT event (through INTENCLR.PORT).
   *   2. Configure the sources (PIN_CNF[n].SENSE).
   *   3. Clear any potential event that could have occurred during
   * configuration (write '0' to EVENTS_PORT).
   *   4. Enable interrupts (through INTENSET.PORT).
   */
  INTENCLR |= 1 << PORT_EVENT;

  for (int i = 0; i < switch_pins_size; i++) {
    GPIO_DIR |= (0 << switch_pins[i]);
    PIN_CNF(switch_pins[i]) = CNF_SENSE_HIGH;
  }

  EVENTS_PORT = 0;
  INTENSET |= 1 << PORT_EVENT;
}

void GPIOTE_IRQHandler() {
  if (EVENTS_PORT) {
    EVENTS_PORT = 0;

    for (int k = 0; k < 10; k++) {
      blink_leds(50000);
    }
  }
}