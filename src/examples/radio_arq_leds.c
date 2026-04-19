#include <stddef.h>
#include <stdint.h>

#include "examples.h"
#include "keyscan.h"
#include "leds.h"
#include "probe.h"
#include "radio.h"
#include "radio_arq.h"
#include "utils.h"

static uint8_t led_gpios[] = {LED_PIN_2, LED_PIN_4, LED_PIN_3, LED_PIN_1};
static size_t led_gpios_size = sizeof(led_gpios) / sizeof(uint8_t);

static uint32_t report;

static keyscan_gpios_t matrix = {
    .direct = {SW_PIN_1, SW_PIN_2, SW_PIN_3, SW_PIN_4},
    .direct_len = 4,
};

int example_radio_arq_leds() {
  init_leds(led_gpios, led_gpios_size);
  init_radio();

  leds_blink();

#ifdef RADIO_RX
  leds_set(0, 1);

  while (1) {
    radio_arq_receive(&report, sizeof(report));
    leds_set_all(report);
  }
#endif
  init_keyscan_direct(0, &matrix);

  while (1) {
    leds_set(3, 1);
    __asm__("WFI");
  }
}

#ifdef EXAMPLE_RADIO_ARQ_LEDS
void KEYSCAN_EventHandler(uint8_t keyscan_id, keyscan_state_t state) {
  probe_pulse_times(probe_tag_switch_handler, 3);
  radio_arq_send(&state.matrix[0], sizeof(state.matrix[0]));
  leds_set_all(state.matrix[0]);
  probe_pulse_times(probe_tag_switch_handler, 1);
}
#endif
