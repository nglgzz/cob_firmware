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
    gpio_mode(0, led_gpios[i], GPIO_DIR_PIN0_Output);
    // Clear LED, setting pin to high as the LED is pulled up.
    gpio_write(0, led_gpios[i], 1);
  }
}

// State is 1 = on, 0 = off
void leds_set(int index, int on) {
  if (on) {
    gpio_write(0, led_gpios[index % led_gpios_size], 0);
  } else {
    gpio_write(0, led_gpios[index % led_gpios_size], 1);
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
    delay(800000);
    leds_set(i, 0);
  }
}
