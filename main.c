#include <stdint.h>

#include "leds.h"
#include "switches.h"

int main(void) {
  init_switches();
  init_leds();

  while (1) {
    blink_leds(500000);
  }
}