#include <stdint.h>

#include "examples.h"
#include "gpio.h"
#include "leds.h"
#include "timer.h"

#ifdef BOARD2
static uint8_t led_gpios[] = {29, 31};
static uint8_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);
#else
static uint8_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static uint8_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);
#endif

int example_blinky() {
  init_leds(led_gpios, led_gpios_size);

  leds_blink();

  while (1) {
    leds_blink();
  }
}
