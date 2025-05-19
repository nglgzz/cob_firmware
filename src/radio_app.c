#include "radio_app.h"

#include "leds.h"
#include "probe.h"

static volatile radio_packet_t send_payload = {.len = 10, .data = {0}};

void start_tx_loop() {
  for (int i = 0; i < send_payload.len; i++) {
    send_payload.data[i] = i;
  }

  radio_send(&send_payload);
}

void start_rx_loop() { radio_receive(); }

void RADIO_ReceivedHandler(volatile radio_packet_t* payload) {
  if (RADIO->CRCSTATUS == 1) {
    // CRC succeeded
    toggle_led(payload->data[0], 1);
    delay(5000000);
    toggle_led(payload->data[0], 0);
  } else {
    // CRC failed
  }

  // Restart RX
  radio_receive();
};

void RADIO_SentHandler(volatile radio_packet_t* payload) {
  // Show payload
  toggle_led(payload->data[0], 1);
  delay(5000000);
  toggle_led(payload->data[0], 0);

  // Update payload
  payload->data[0]++;

  // Restart TX
  radio_send(&send_payload);
};

// PP_D0 TXEN -> END: Overall ramp-up and transmission
// PP_D1 TXEN -> READY: Ramp-up
// PP_D2 READY -> END: Transmission
// PP_D3 TASKS_TXEN
// PP_D4 EVENTS_READY
// PP_D5 EVENTS_END
// void handle_send_events() {
//   if (RADIO->EVENTS_READY) {
//     RADIO->EVENTS_READY = 0;

//     probe_pulse(PP_D4);  // EVENTS_READY triggered
//     probe_off(PP_D1);    // Ramp-up done
//     probe_on(PP_D2);     // Sending start
//   }
//   if (RADIO->EVENTS_END) {
//     RADIO->EVENTS_END = 0;

//     probe_pulse(PP_D5);  // EVENTS_END triggered
//     probe_off(PP_D0);    // Overall done
//     probe_off(PP_D2);    // Sending done

//     toggle_led(tx_payload.data[0], 1);
//     delay(5000000);
//     toggle_led(tx_payload.data[0], 0);

//     // Update payload
//     tx_payload.data[0]++;

//     // Restart TX
//     probe_pulse(PP_D3);  // TXEN triggered
//     probe_on(PP_D0);     // Overall start
//     probe_on(PP_D1);     // Ramp-up start

//     RADIO->PACKETPTR = (uint32_t)&tx_payload;
//     radio_start_tx();
//   }
// }

// PP_D0 READY
// PP_D1 ADDRESS
// PP_D2 PAYLOAD
// PP_D3 END
// PP_D3 BCMATCH
// void handle_receive_events() {
//   if (RADIO->EVENTS_READY) {
//     RADIO->EVENTS_READY = 0;
//     probe_pulse(PP_D0);
//   }
//   if (RADIO->EVENTS_ADDRESS) {
//     RADIO->EVENTS_ADDRESS = 0;
//     probe_pulse(PP_D1);
//   }
//   if (RADIO->EVENTS_PAYLOAD) {
//     RADIO->EVENTS_PAYLOAD = 0;
//     probe_pulse(PP_D2);
//   }
//   if (RADIO->EVENTS_END) {
//     RADIO->EVENTS_END = 0;
//     probe_pulse(PP_D3);

//     if (RADIO->CRCSTATUS == 1) {
//       // CRC succeeded
//       toggle_led(.data[0], 1);
//       delay(5000000);
//       toggle_led(rx_payload.data[0], 0);
//     } else {
//       // CRC failed
//     }

//     // Restart RX
//     RADIO->EVENTS_READY = 0;
//     RADIO->EVENTS_ADDRESS = 0;
//     RADIO->EVENTS_PAYLOAD = 0;
//     RADIO->EVENTS_END = 0;

//     RADIO->PACKETPTR = (uint32_t)&rx_payload;
//     radio_start_rx();
//   }
// }
