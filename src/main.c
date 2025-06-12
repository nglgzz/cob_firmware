#include <stdint.h>

#include "core.h"
#include "leds.h"
#include "matrix_scan.h"
#include "probe.h"
#include "radio.h"
#include "switches.h"
#include "usbd.h"
#include "utils.h"

int main(void) {
  /**
   * [1] SLEEPONEXIT - When exiting a thread, for example an interrupt handler,
   * go into sleep instead of returning into the main function.
   *
   * [2] SLEEPDEEP - Go into the lowest power sleep
   *
   * Note: PRIMASK and FAULTMASK might also need to be set for external
   * interrupts to work properly when they have higher priority than the current
   * exception (so potential startup code is run before the external interrupt
   * handler).
   */
  SCR = 1 << 1 | 1 << 2;

#ifdef RADIO_RX
  init_usbd();
#endif

  init_probes();
  init_switches();
  init_leds();
  init_radio();

  // Blink LEDs to signal we're executing the main function and the
  // initialization is done.
  blink_leds(500000);
  blink_leds(500000);

#ifdef RADIO_RX
  receive();
#endif

  while (1) {
    // LED 2 will stay off unless SWITCH 2 is being pressed or unless we return
    // to the main function after completing the execution of an interrupt
    // handler.
    toggle_led(0, 0);
    __asm__("WFI");

    // As long as the deep sleep configuration is correct, this should be
    // unreacheable code. If there is some misconfiguration, LED 2 will turn on.
    delay(5000000);
    toggle_led(0, 1);
    delay(5000000);
  }
}
