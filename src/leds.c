/**
 * NOTES:
 *    - Max number of LEDs is determined by MAX_LED_PINS_LEN.
 *    - Each GPIO pin is defined as a two byte number, where the MSB is the GPIO
 *      port, and the LSB is the GPIO pin.
 */
#include "leds.h"

#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "nrf52840_bitfields.h"
#include "utils.h"

uint16_t led_gpios[MAX_LED_PINS_LEN];
size_t led_gpios_len;

void init_leds(uint16_t pins[], size_t pins_len) {
  led_gpios_len = pins_len <= MAX_LED_PINS_LEN ? pins_len : MAX_LED_PINS_LEN;
  memcpy(led_gpios, pins, led_gpios_len * sizeof(uint16_t));

  for (int i = 0; i < led_gpios_len; i++) {
    gpio_mode(led_gpios[i] >> 8, led_gpios[i] & 0xFF, GPIO_DIR_PIN0_Output);
    // Clear LED, setting pin to high as the LED is pulled up.
    gpio_write(led_gpios[i] >> 8, led_gpios[i] & 0xFF, GPIO_OUT_PIN0_High);
  }
}

// State is 1 = on, 0 = off
void leds_set(int index, int on) {
  if (on) {
    gpio_write(0, led_gpios[index % led_gpios_len], 0);
  } else {
    gpio_write(0, led_gpios[index % led_gpios_len], 1);
  }
}

void leds_set_all(uint32_t leds) {
  for (int i = 0; i < led_gpios_len; i++) {
    if (leds & (1 << i)) {
      leds_set(i, 1);
    } else {
      leds_set(i, 0);
    }
  }
}

void leds_blink() {
  for (int i = 0; i < led_gpios_len; i++) {
    leds_set(i, 1);
    delay(500000);
    leds_set(i, 0);
  }
}
