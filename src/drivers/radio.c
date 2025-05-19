#include "radio.h"

#include "clock.h"
#include "core.h"
#include "leds.h"
#include "probe.h"

struct radio *RADIO = ((struct radio *)RADIO_BASE);

#define PAYLOAD_LEN 100

// Packet addresses to be used for the next transmission or reception. When
// transmitting, the packet pointed to by this address will be transmitted and
// when receiving, the received packet will be written to this address. This
// address is a byte aligned RAM address.
static volatile radio_packet_t tx_packet = {.len = PAYLOAD_LEN, .data = {0}};
static volatile radio_packet_t rx_packet = {.len = PAYLOAD_LEN, .data = {0}};

void init_radio() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  // The radio is on by default, but doesn't hurt to be explicit.
  RADIO->POWER = 1;
  // Fixed on channel 8 for now, so 2408 MHz
  // TODO: figure out channel hopping
  RADIO->FREQUENCY = 80;
  // https://en.wikipedia.org/wiki/DBm
  // 4dBm - 2.5mW - Bluetooth Class 2 radio, 10 m range
  // 0dBm - 1.0mW - Bluetooth standard (Class 3) radio, 1 m range
  RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_0dBm;
  // Higher power is easier to notice in the power profiler
  // RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_Pos4dBm;

  // Going for 1Mbps mode for now as I assume has the lowest power consumption
  // and delay
  RADIO->MODE = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

  // The packet configuration is the following: 8bits length field (not
  // including the CRC length), no S0 and S1 fields, 8 bit long preamble ...
  RADIO->PCNF0 = (RADIO_PCNF0_PLEN_8bit << RADIO_PCNF0_PLEN_Pos) |
                 (8 << RADIO_PCNF0_LFLEN_Pos) |
                 (RADIO_PCNF0_CRCINC_Include << RADIO_PCNF0_CRCINC_Pos) |
                 (0 << RADIO_PCNF0_S0LEN_Pos) | (0 << RADIO_PCNF0_S1LEN_Pos);
  // ... the base address is 2 bytes long (4 hex digits), the payload has a max
  // length of 255 bytes, static length of 32 bytes, and whitening is enabled
  RADIO->PCNF1 = (3 << RADIO_PCNF1_BALEN_Pos) |
                 (RADIO_PAYLOAD_MAXLEN << RADIO_PCNF1_MAXLEN_Pos) |
                 (0 << RADIO_PCNF1_STATLEN_Pos) |
                 (RADIO_PCNF1_WHITEEN_Enabled << RADIO_PCNF1_WHITEEN_Pos) |
                 (RADIO_PCNF1_ENDIAN_Little << RADIO_PCNF1_ENDIAN_Pos);

  // Data whitening initial value
  RADIO->DATAWHITEIV = 0x55;

  // CRC is enabled and has length of 2 bytes
  RADIO->CRCCNF = RADIO_CRCCNF_LEN_Disabled << RADIO_CRCCNF_LEN_Pos;
  // RADIO->CRCCNF = RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos;
  // RADIO->CRCINIT = 0xFFFF;
  // RADIO->CRCPOLY = 0x11021;

  // TX & RX: use logical address 0
  RADIO->BASE0 = 0XE7E7E7E7;
  RADIO->PREFIX0 = 0x00;
  RADIO->TXADDRESS = 0;
  RADIO->RXADDRESSES = 0x01;

  RADIO->SHORTS = RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk;

  // Enable generating interrupts for events
  RADIO->INTENSET = RADIO_INTENSET_READY_Msk | RADIO_INTENSET_ADDRESS_Msk |
                    RADIO_INTENSET_PAYLOAD_Msk | RADIO_INTENSET_END_Msk;

  // Enable the RADIO interrupt request handler. If this is not set, the
  // peripheral can still generate interrupts, but they end up permanently
  // pending as the handlers are not executed.
  NVIC_SETENA |= 1 << RADIO_IRQn;
}

void radio_receive() {
  // Clear any pending events
  RADIO->EVENTS_READY = 0;
  RADIO->EVENTS_ADDRESS = 0;
  RADIO->EVENTS_PAYLOAD = 0;
  RADIO->EVENTS_END = 0;

  RADIO->PACKETPTR = (uint32_t)&rx_packet;
  RADIO->TASKS_RXEN = 1;
}

void radio_send(volatile radio_packet_t *payload) {
  RADIO->PACKETPTR = (uint32_t)payload;

  // Clear events
  RADIO->EVENTS_READY = 0;
  RADIO->EVENTS_END = 0;

  // Start TX task
  probe_pulse(PP_D3);  // TXEN triggered
  probe_on(PP_D0);     // start overall transmission
  probe_on(PP_D1);     // ramp-up start

  RADIO->TASKS_TXEN = 1;
}

void noop(volatile radio_packet_t *payload) {}
void RADIO_ReceivedHandler(volatile radio_packet_t *payload)
    __attribute__((weak, alias("noop")));
void RADIO_SentHandler(volatile radio_packet_t *payload) __attribute__((weak, alias("noop")));

void RADIO_IRQHandler() {
  if (RADIO->EVENTS_READY) {
    RADIO->EVENTS_READY = 0;
  }
  if (RADIO->EVENTS_ADDRESS) {
    RADIO->EVENTS_ADDRESS = 0;
  }
  if (RADIO->EVENTS_PAYLOAD) {
    RADIO->EVENTS_PAYLOAD = 0;
  }
  if (RADIO->EVENTS_END) {
    RADIO->EVENTS_END = 0;

#ifdef RADIO_RX
    RADIO_ReceivedHandler((volatile radio_packet_t *)RADIO->PACKETPTR);
#endif

#ifdef RADIO_TX
    RADIO_SentHandler((volatile radio_packet_t *)RADIO->PACKETPTR);
#endif
  }
}
