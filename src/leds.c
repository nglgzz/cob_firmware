/**
 * NOTES:
 *    - Max number of LEDs is determined by MAX_LED_PINS_SIZE.
 *    - All LEDs need to be on GPIO port 0.
 */
#include "leds.h"

#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "nrf52840_bitfields.h"
#include "utils.h"

uint8_t led_pins[MAX_LED_PINS_SIZE];
size_t led_pins_size;

void init_leds(uint8_t pins[], size_t pins_size) {
  led_pins_size = pins_size <= MAX_LED_PINS_SIZE ? pins_size : MAX_LED_PINS_SIZE;
  memcpy(led_pins, pins, led_pins_size);

  for (int i = 0; i < led_pins_size; i++) {
    // toggling GPIO_DIR at pin number (led_pins[i]) to high
    // which makes all led_pins output pins.
    GPIO0->DIRSET = (GPIO_DIRSET_PIN0_Set << led_pins[i]);
    // Clear LED.
    GPIO0->OUTSET = (GPIO_OUTSET_PIN0_Set << led_pins[i]);
  }
}

// State is 1 = on, 0 = off
void leds_set(int index, int on) {
  if (on) {
    GPIO0->OUTCLR = (GPIO_OUTCLR_PIN0_Clear << led_pins[index % led_pins_size]);
  } else {
    GPIO0->OUTSET = (GPIO_OUTSET_PIN0_Set << led_pins[index % led_pins_size]);
  }
}

void leds_set_all(uint32_t leds) {
  for (int i = 0; i < led_pins_size; i++) {
    if (leds & (1 << i)) {
      leds_set(i, 1);
    } else {
      leds_set(i, 0);
    }
  }
}

void leds_blink() {
  for (int i = 0; i < led_pins_size; i++) {
    leds_set(i, 1);
    delay(500000);
    leds_set(i, 0);
  }
}
