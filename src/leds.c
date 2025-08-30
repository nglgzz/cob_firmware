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

uint8_t led_gpios[MAX_LED_PINS_SIZE];
size_t led_gpios_size;

void init_leds(uint8_t pins[], size_t pins_size) {
  led_gpios_size = pins_size <= MAX_LED_PINS_SIZE ? pins_size : MAX_LED_PINS_SIZE;
  memcpy(led_gpios, pins, led_gpios_size);

  for (int i = 0; i < led_gpios_size; i++) {
    // toggling GPIO_DIR at pin number (led_gpios[i]) to high
    // which makes all led_gpios output pins.
    GPIO0->DIRSET = (GPIO_DIRSET_PIN0_Set << led_gpios[i]);
    // Clear LED.
    GPIO0->OUTSET = (GPIO_OUTSET_PIN0_Set << led_gpios[i]);
  }
}

// State is 1 = on, 0 = off
void leds_set(int index, int on) {
  if (on) {
    GPIO0->OUTCLR = (GPIO_OUTCLR_PIN0_Clear << led_gpios[index % led_gpios_size]);
  } else {
    GPIO0->OUTSET = (GPIO_OUTSET_PIN0_Set << led_gpios[index % led_gpios_size]);
  }
}

void leds_set_all(uint32_t leds) {
  for (int i = 0; i < led_gpios_size; i++) {
    if (leds & (1 << i)) {
      leds_set(i, 1);
    } else {
      leds_set(i, 0);
    }
  }
}

void leds_blink() {
  for (int i = 0; i < led_gpios_size; i++) {
    leds_set(i, 1);
    delay(500000);
    leds_set(i, 0);
  }
}
