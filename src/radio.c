#include "radio.h"

#include "core.h"
#include "leds.h"
#include "probe.h"

struct radio *RADIO = ((struct radio *)RADIO_BASE);

#define PBUF_LEN 128
#define PAYLOAD_LEN 100
typedef struct {
  uint8_t len;
  uint8_t data[PBUF_LEN];
} radio_packet_t;
static volatile radio_packet_t tx_packet = {.len = PAYLOAD_LEN, .data = {0}};
static volatile radio_packet_t rx_packet = {.len = PAYLOAD_LEN, .data = {0}};

void init_radio() {
  // The radio is on by default, but doesn't hurt to be explicit.
  RADIO->POWER = 1;
  // Fixed on channel 8 for now, so 2408 MHz
  // TODO: figure out channel hopping
  RADIO->FREQUENCY = 80;
  // https://en.wikipedia.org/wiki/DBm
  // 4dBm - 2.5mW - Bluetooth Class 2 radio, 10 m range
  // 0dBm - 1.0mW - Bluetooth standard (Class 3) radio, 1 m range
  // RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_0dBm;
  // Higher power is easier to notice in the power profiler
  RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos4dBm;

  // Going for 1Mbps mode for now as I assume has the lowest power consumption
  // and delay
  RADIO->MODE = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

  // The packet configuration is the following: 8bits length field (not
  // including the CRC length), no S0 and S1 fields, 8 bit long preamble ...
  RADIO->PCNF0 = (RADIO_PCNF0_PLEN_8bit << RADIO_PCNF0_PLEN_Pos) |
                 (0 << RADIO_PCNF0_LFLEN_Pos) |
                 (RADIO_PCNF0_CRCINC_Include << RADIO_PCNF0_CRCINC_Pos) |
                 (0 << RADIO_PCNF0_S0LEN_Pos) | (0 << RADIO_PCNF0_S1LEN_Pos);
  // ... the base address is 2 bytes long (4 hex digits), the payload has a max
  // length of 255 bytes, static length of 32 bytes, and whitening is enabled
  RADIO->PCNF1 = (2 << RADIO_PCNF1_BALEN_Pos) | (PBUF_LEN << RADIO_PCNF1_MAXLEN_Pos) |
                 (PAYLOAD_LEN << RADIO_PCNF1_STATLEN_Pos) |
                 (RADIO_PCNF1_WHITEEN_Enabled << RADIO_PCNF1_WHITEEN_Pos) |
                 (RADIO_PCNF1_ENDIAN_Little << RADIO_PCNF1_ENDIAN_Pos);

  // CRC is enabled and has length of 2 bytes
  RADIO->CRCCNF = RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos;
  RADIO->CRCINIT = 0xFFFF;
  RADIO->CRCPOLY = 0x11021;

  RADIO->SHORTS = RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;

  // Packet address to be used for the next transmission or reception. When
  // transmitting, the packet pointed to by this address will be transmitted and
  // when receiving, the received packet will be written to this address. This
  // address is a byte aligned RAM address.
#ifdef RADIO_RX
  RADIO->PACKETPTR = (uint32_t)&rx_packet;
#endif

#ifdef RADIO_TX
  RADIO->PACKETPTR = (uint32_t)&tx_packet;
#endif

  // Enable generating interrupts for END events
  // RADIO->INTENSET = RADIO_INTENSET_END_Msk | RADIO_INTENSET_READY_Msk |
  //                   RADIO_INTENSET_ADDRESS_Msk | RADIO_INTENSET_PAYLOAD_Msk;

  // Enable the RADIO interrupt request handler. If this is not set, the
  // peripheral can still generate interrupts, but they end up permanently
  // pending as the handlers are not executed.
  NVIC_SETENA |= 1 << RADIO_IRQn;
}

void init_radio_rx() {
  // Set base address to something neutral
  RADIO->BASE0 = 0XE7E7E7E7;
  RADIO->PREFIX0 = 0xE7;

  // Set logical address 0
  RADIO->TXADDRESS = 0;
  // Enable pipe 0
  RADIO->RXADDRESSES = 0x01;

  // Enable generating interrupts for END events
  // RADIO->INTENSET = RADIO_INTENSET_END_Msk;
  RADIO->INTENSET = RADIO_INTENSET_END_Msk | RADIO_INTENSET_ADDRESS_Msk |
                    RADIO_INTENSET_PAYLOAD_Msk | RADIO_INTENSET_RSSIEND_Msk |
                    RADIO_INTENSET_READY_Msk;

  // Start receiver
  RADIO->TASKS_RXEN = 1;
}

void handle_receive_events() {
  if (RADIO->EVENTS_RSSIEND) {
    while (RADIO->EVENTS_RSSIEND == 0);
    int8_t rssi = RADIO->RSSISAMPLE;
    RADIO->EVENTS_RSSIEND = 0;
    // RADIO->TASKS_RSSISTART = 1;
  }
  if (RADIO->EVENTS_READY) {
    RADIO->EVENTS_READY = 0;
    // RADIO->TASKS_RSSISTART = 1;
  }
  if (RADIO->EVENTS_ADDRESS) {
    RADIO->EVENTS_ADDRESS = 0;
  }
  if (RADIO->EVENTS_PAYLOAD) {
    RADIO->EVENTS_PAYLOAD = 0;
  }
  if (RADIO->EVENTS_END) {
    RADIO->EVENTS_END = 0;

    if (RADIO->CRCSTATUS == 1) {
      // CRC succeeded
      toggle_led(rx_packet.data[0], 1);
      delay(50000);
      toggle_led(rx_packet.data[0], 0);
      // Can read rx_packet here
    } else {
      // CRC failed
    }

    // Restart RX
    // TODO - not sure if I need this
    RADIO->TASKS_RXEN = 1;
  }
}

void init_radio_tx() {
  // Set base address to something neutral
  RADIO->BASE0 = 0XE7E7E7E7;
  RADIO->PREFIX0 = 0xE7;

  // Set logical address 0
  RADIO->TXADDRESS = 0;
  // Enable pipe 0
  RADIO->RXADDRESSES = 0x01;

  // Enable generating interrupts for END events
  RADIO->INTENSET = RADIO_INTENSET_END_Msk | RADIO_INTENSET_READY_Msk;
}

void start_tx_loop() {
  for (int i = 0; i < tx_packet.len; i++) {
    tx_packet.data[i] = i;
  }

  // Clear events
  RADIO->EVENTS_READY = 0;
  RADIO->EVENTS_END = 0;

  // Start TX task
  probe(PP_D3);     // TXEN triggered
  probe_on(PP_D0);  // start overall transmission
  probe_on(PP_D1);  // ramp-up start
  RADIO->TASKS_TXEN = 1;
}

// PP_D0 TXEN -> END: Overall ramp-up and transmission
// PP_D1 TXEN -> READY: Ramp-up
// PP_D2 READY -> END: Transmission
// PP_D3 TASKS_TXEN
// PP_D4 EVENTS_READY
// PP_D5 EVENTS_END
void handle_send_events() {
  if (RADIO->EVENTS_READY) {
    RADIO->EVENTS_READY = 0;

    probe(PP_D4);      // EVENTS_READY triggered
    probe_off(PP_D1);  // Ramp-up done
    probe_on(PP_D2);   // Sending start
  }
  if (RADIO->EVENTS_END) {
    RADIO->EVENTS_END = 0;

    probe(PP_D5);      // EVENTS_END triggered
    probe_off(PP_D0);  // Overall done
    probe_off(PP_D2);  // Sending done

    // Update payload
    tx_packet.data[0]++;
    toggle_led(tx_packet.data[0], 1);
    delay(500000);
    toggle_led(tx_packet.data[0], 0);

    // Restart TX
    probe(PP_D3);     // TXEN triggered
    probe_on(PP_D0);  // Overall start
    probe_on(PP_D1);  // Ramp-up start

    RADIO->PACKETPTR = (uint32_t)&tx_packet;
    RADIO->TASKS_TXEN = 1;
  }
}

void RADIO_IRQHandler() {
#ifdef RADIO_RX
  handle_receive_events();
#endif

#ifdef RADIO_TX
  handle_send_events();
#endif
}
