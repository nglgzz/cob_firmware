#include <stdint.h>

#include "leds.h"
#include "nrf52840.h"
#include "switches.h"

int main(void) {
  NVIC_SETENA = 1 << GPIOTE_IRQn;

  init_switches();
  init_leds();

  while (1) {
    blink_leds(500000);
  }
}