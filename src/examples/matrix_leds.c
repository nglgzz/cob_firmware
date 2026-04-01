#include <stddef.h>
#include <stdint.h>

#include "core.h"
#include "examples.h"
#include "gpio.h"
#include "gpiote.h"
#include "leds.h"
#include "nrf52840_bitfields.h"

static uint16_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_len = sizeof(led_gpios) / sizeof(uint16_t);

static uint8_t col_pin = 10;
static uint8_t row_pin = 5;

int example_matrix_leds() {
  init_leds(led_gpios, led_gpios_len);

  leds_blink();
  leds_set(0, 1);

  NVIC_EnableIRQ(GPIOTE_IRQn);
  gpiote_disable_port_events();

  gpio_mode(1, col_pin, GPIO_PIN_CNF_DIR_Output);
  gpio_write(1, col_pin, 1);

  gpio_set_cnf(1,
               row_pin,
               (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                   (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                   (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos) |
                   (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos));
  gpiote_enable_port_events();

  while (1) {
    uint8_t col_state = gpio_read(1, col_pin);
    uint8_t row_state = gpio_read(1, row_pin);

    __asm__("WFI");
  }
}

#ifdef EXAMPLE_MATRIX_LEDS

void GPIOTE_PortEventHandler() {
  uint8_t col_value = gpio_read(1, col_pin);
  uint8_t row_value = gpio_read(1, row_pin);

  if (col_value == 0 && row_value == 1) {
    leds_set(0, 1);
    leds_set(1, 1);
    gpio_set_cnf(1,
                 row_pin,
                 (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                     (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos) |
                     (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos));
  } else {
    leds_set(0, 0);
    leds_set(1, 0);
    gpio_set_cnf(1,
                 row_pin,
                 (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos) |
                     (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos) |
                     (GPIO_PIN_CNF_PULL_Pulldown << GPIO_PIN_CNF_PULL_Pos) |
                     (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos));
  }
}

#endif
