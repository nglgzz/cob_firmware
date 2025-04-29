#include "switches.h"

#include "core.h"
#include "gpio.h"
#include "gpiote.h"
#include "leds.h"

static uint16_t switch_pins[] = {SW_PIN_2, SW_PIN_4, SW_PIN_3, SW_PIN_1};
static size_t switch_pins_size = sizeof(switch_pins) / sizeof(switch_pins[0]);

struct gpio_pin_cnf pin_sense_low = {
    GPIO_CNF_DIR_INPUT,
    GPIO_CNF_INPUT_CONNECT,
    GPIO_CNF_PULL_PULLUP,
    GPIO_CNF_SENSE_LOW,
};

struct gpio_pin_cnf pin_sense_high = {
    GPIO_CNF_DIR_INPUT,
    GPIO_CNF_INPUT_CONNECT,
    GPIO_CNF_PULL_PULLUP,
    GPIO_CNF_SENSE_HIGH,
};

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
  GPIOTE->INTENCLR |= 1 << PORT_EVENT;

  for (int i = 0; i < switch_pins_size; i++) {
    GPIO0->DIR |= (0 << switch_pins[i]);

    gpio0_set_pin_cnf(switch_pins[i], &pin_sense_low);
  }

  // Clear PORT events
  GPIOTE->EVENTS_PORT = 0;

  // Enable interrupts
  GPIOTE->INTENSET |= 1 << PORT_EVENT;
}

void GPIOTE_IRQHandler() {
  if (GPIOTE->EVENTS_PORT) {
    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;

    for (int i = 0; i < switch_pins_size; i++) {
      uint16_t pin = switch_pins[i];
      uint32_t pinValue = GPIO0->IN & (1 << pin);

      if (pinValue) {
        toggle_led(i, 0);
        gpio0_set_pin_cnf(pin, &pin_sense_low);
      } else {
        toggle_led(i, 1);
        gpio0_set_pin_cnf(pin, &pin_sense_high);
      }
    }

    // Clear PORT events
    GPIOTE->EVENTS_PORT = 0;
  }
}
