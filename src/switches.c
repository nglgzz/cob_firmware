#include "switches.h"

#include <stddef.h>

#include "core.h"
#include "gpio.h"
#include "gpiote.h"
#include "leds.h"
#include "matrix_scan.h"
#include "nrf52840_bitfields.h"
#include "usbd_hid.h"

static uint16_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(switch_pins[0]);

static const uint32_t sense_low = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                  (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                  (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                  (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos);

static const uint32_t sense_high = (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                                   (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) |
                                   (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos);

void init_switches() {
  // Enable the GPIOTE interrupt request handler. If this is not set, the
  // peripheral can still generate interrupts, but they end up permanently
  // pending as the handlers are not executed.
  NVIC_EnableIRQ(GPIOTE_IRQn);

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
  GPIOTE->INTENCLR |= GPIOTE_INTENCLR_PORT_Msk;

  for (int i = 0; i < switch_pins_size; i++) {
    GPIO0->DIRCLR = (GPIO_DIRCLR_PIN0_Clear << switch_pins[i]);
    GPIO0->PIN_CNF[switch_pins[i]] = sense_low;
  }

  // Clear PORT events
  GPIOTE->EVENTS_PORT = 0;

  // Enable interrupts
  GPIOTE->INTENSET |= GPIOTE_INTENSET_PORT_Msk;
}

void GPIOTE_IRQHandler() {
  if (GPIOTE->EVENTS_PORT) {
    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;
    uint32_t switches = 0x00;

    for (int i = 0; i < switch_pins_size; i++) {
      uint16_t pin = switch_pins[i];
      uint32_t pinValue = GPIO0->IN & (GPIO_IN_PIN0_High << pin);

      // The values are flipped because the switches are pulled up (i.e. 1 is low and 0 is
      // high).
      switches |= (pinValue ? 0U : 1U) << i;

      if (pinValue) {
        GPIO0->PIN_CNF[switch_pins[i]] = sense_low;
      } else {
        GPIO0->PIN_CNF[switch_pins[i]] = sense_high;
      }
    }
    send(switches);
    usbd_hid_send(switches);

    // Clear potential PORT events that could have occurred during configuration.
    GPIOTE->EVENTS_PORT = 0;
  }
}
