#include "matrix_scan.h"

#include "gpio.h"
#include "leds.h"
#include "probe.h"
#include "radio.h"
#include "utils.h"

static radio_packet_t packet = {.len = 48, .data = {0}};

typedef struct __attribute__((packed)) {
  // 7 6 5 4 3 2 1 0
  // _ B B B _ A A A
  //
  // A: Device ID
  // B: Device type
  //      - 0: split left
  //      - 1: split right
  //      - 2: macro pad
  //      - 3: unibody
  //      - 4: mouse
  //      - 5: touchpad
  //      - 6: headset
  uint8_t device;
  uint32_t pins;
  uint8_t port;
} switch_report_t;

static switch_report_t report = {
    .device = 0x00,
    .pins = 0x00000000,
    .port = 0,
};

void send(uint32_t switches) {
  report.device = 0x00;
  report.pins = switches;
  report.port = 0;

  memcpy((void*)packet.data, &report, sizeof(report));
  radio_send(&packet);
}
void RADIO_SentHandler(volatile radio_packet_t* payload) {
  for (int i = 0; i < 4; i++) {
    if (report.pins & (1 << i)) {
      toggle_led(i, 1);
    } else {
      toggle_led(i, 0);
    }
  }
};

void receive() { radio_receive(); }
void RADIO_ReceivedHandler(volatile radio_packet_t* payload) {
  if (RADIO->CRCSTATUS == 1) {
    memcpy((void*)&report, (void*)payload->data, sizeof(report));

    for (int i = 0; i < 4; i++) {
      toggle_led(i, report.pins & (1U << i));
    }
  }
  radio_receive();
};
