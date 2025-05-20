#include "leds.h"

// stddef is defining size_t, while stdint is defining fixed width integer
// types, both are used in the "leds.c" file.
#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "utils.h"

// uintn_t is a fixed width integer, the size of a regular int is implementation
// dependent.
static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
// size_t is a type used to represent the size of an object, most functions that
// deal with sizes, expect or return this type.
static size_t led_pins_size = sizeof(led_pins) / sizeof(led_pins[0]);

void init_leds() {
  for (int i = 0; i < led_pins_size; i++) {
    // toggling GPIO_DIR at pin number (led_pins[i]) to high
    // which makes all led_pins output pins.
    GPIO0->DIRSET = (GPIO_DIRSET_Set << led_pins[i]);
    // Clear LED.
    GPIO0->OUTSET = (GPIO_OUTSET_Set << led_pins[i]);
  }
}

// State is 1 = on, 0 = off
void toggle_led(int index, int state) {
  if (state) {
    GPIO0->OUTCLR = (GPIO_OUTCLR_Clear << led_pins[index % led_pins_size]);
  } else {
    GPIO0->OUTSET = (GPIO_OUTSET_Set << led_pins[index % led_pins_size]);
  }
}

void blink_leds(uint32_t duration) {
  for (int i = 0; i < led_pins_size; i++) {
    toggle_led(i, 1);
    delay(duration);
    toggle_led(i, 0);
  }
}
