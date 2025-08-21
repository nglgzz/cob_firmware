#include <stdint.h>

#include "core.h"
#include "examples.h"
#include "leds.h"
#include "probe.h"
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
#ifndef RADIO_RX
  SCR = 1 << 1 | 1 << 2;
#endif

  init_probes();

  //------------------
  // EXAMPLES
  //------------------

#ifdef EXAMPLE_BLINKY
  example_blinky();
#elif defined(EXAMPLE_SWITCHES_LEDS)
  example_switches_leds();
#elif defined(EXAMPLE_RADIO_LEDS)
  example_radio_leds();
#elif defined(EXAMPLE_RADIO_ARQ_LEDS)
  example_radio_arq_leds();
#elif defined(EXAMPLE_RADIO_AUTO_TX)
  example_radio_auto_tx();
#elif defined(EXAMPLE_RADIO_HID)
  example_radio_hid();
#endif

  while (1) {
    // LED 2 will stay off unless SWITCH 2 is being pressed or unless we return
    // to the main function after completing the execution of an interrupt
    // handler.
    leds_set(0, 0);
    __asm__("WFI");

    // As long as the deep sleep configuration is correct, this should be
    // unreacheable code. If there is some misconfiguration, LED 2 will turn on.
    delay(5000000);
    leds_set(0, 1);
    delay(5000000);
  }
}
