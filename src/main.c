#include <stdint.h>

#include "core.h"
#include "examples.h"
#include "leds.h"
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
#ifndef EXAMPLE_RADIO_AUTO_TX
#ifndef RADIO_RX
  SCR = 1 << 1 | 1 << 2;
#endif
#endif

  //------------------
  // EXAMPLES
  //------------------

#ifdef EXAMPLE_BLINKY
  example_blinky();
#elif defined(EXAMPLE_SWITCHES_LED)
  example_switches_led();
#elif defined(EXAMPLE_SWITCHES_RADIO)
  example_switches_radio();
#elif defined(EXAMPLE_SWITCHES_RADIO_HID)
  example_switches_radio_hid();
#elif defined(EXAMPLE_RADIO_AUTO_TX)
  example_radio_auto_tx();
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
