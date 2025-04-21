#include "leds.h"

#include "gpio.h"

// uintn_t is a fixed width integer, the size of a regular int is implementation
// dependent.
static uint8_t led_pins[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
// size_t is a type used to represent the size of an object, most functions that
// deal with sizes, expect or return this type.
static size_t led_pins_size = sizeof(led_pins) / sizeof(led_pins[0]);

void delay(volatile uint32_t duration) { while (duration--); }

void init_leds() {
  for (int i = 0; i < led_pins_size; i++) {
    // toggling GPIO_DIR at pin number (led_pins[i]) to high
    // which makes all led_pins output pins.
    GPIO0->DIR |= (1 << led_pins[i]);
  }
}

static void clear_leds() {
  for (int i = 0; i < led_pins_size; i++) {
    GPIO0->OUTCLR = (1 << led_pins[i]);
  }
}

void toggle_led(int index, int state) {
  if (index < 0 && index >= led_pins_size) {
    // index out of bounds, do nothing
    return;
  }

  if (state) {
    clear_leds();
    GPIO0->OUTSET = (1 << led_pins[index]);
    return;
  }

  GPIO0->OUTCLR = (1 << led_pins[index]);
}

void blink_leds(uint32_t duration) {
  for (int i = 0; i < led_pins_size; i++) {
    // toggle GPIO pin (led_pins[i]) to low
    GPIO0->OUTCLR = (1 << led_pins[i]);
    delay(duration);
    // toggle GPIO pin (led_pins[i]) to high
    GPIO0->OUTSET = (1 << led_pins[i]);
  }
}
