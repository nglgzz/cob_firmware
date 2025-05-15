#include "radio.h"

#include "core.h"

struct radio *RADIO = ((struct radio *)RADIO_BASE);

#define RX_BUF_LEN 255
__attribute__((aligned(4))) uint8_t packet_buffer[RX_BUF_LEN] = {0};

void init_radio() {
  // The radio is on by default, but doesn't hurt to be explicit.
  RADIO->POWER = 1;
  // Fixed on channel 8 for now, so 2408 MHz
  // TODO: figure out channel hopping
  RADIO->FREQUENCY = 8;
  // https://en.wikipedia.org/wiki/DBm
  // 4dBm - 2.5mW - Bluetooth Class 2 radio, 10 m range
  // 0dBm - 1.0mW - Bluetooth standard (Class 3) radio, 1 m range
  RADIO->TXPOWER = RADIO_TXPOWER_TXPOWER_0dBm;
  // Going for 1Mbps mode for now as I assume has the lowest power consumption
  // and delay
  RADIO->MODE = (RADIO_MODE_MODE_Nrf_1Mbit << RADIO_MODE_MODE_Pos);

  // The packet configuration is the following: 8bits length field (not
  // including the CRC length), no S0 and S1 fields, 8 bit long preamble ...
  RADIO->PCNF0 = (RADIO_PCNF0_PLEN_8bit << RADIO_PCNF0_PLEN_Pos) |
                 (8 << RADIO_PCNF0_LFLEN_Pos) |
                 (RADIO_PCNF0_CRCINC_Exclude << RADIO_PCNF0_CRCINC_Pos) |
                 (0 << RADIO_PCNF0_S0LEN_Pos) | (0 << RADIO_PCNF0_S1LEN_Pos);
  // ... the base address is 2 bytes long (4 hex digits), the payload has a max
  // length of 255 bytes, static length of 32 bytes, and whitening is enabled
  RADIO->PCNF1 = (2 << RADIO_PCNF1_BALEN_Pos) | (RX_BUF_LEN << RADIO_PCNF1_MAXLEN_Pos) |
                 (32 << RADIO_PCNF1_STATLEN_Pos) |
                 RADIO_PCNF1_WHITEEN_Enabled << RADIO_PCNF1_WHITEEN_Pos;

  // CRC is enabled and has length of 2 bytes
  RADIO->CRCCNF = RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos;
  RADIO->CRCINIT = 0xFFFF;
  RADIO->CRCPOLY = 0x11021;

  RADIO->SHORTS = RADIO_SHORTS_READY_START_Enabled << RADIO_SHORTS_READY_START_Pos |
                  RADIO_SHORTS_END_DISABLE_Enabled << RADIO_SHORTS_END_DISABLE_Pos;

  // Packet address to be used for the next transmission or reception. When
  // transmitting, the packet pointed to by this address will be transmitted and
  // when receiving, the received packet will be written to this address. This
  // address is a byte aligned RAM address.
  RADIO->PACKETPTR = *packet_buffer;
}

void init_radio_tx() {
  // Set base address to something neutral
  RADIO->BASE0 = 0XE7E7E7E7;
  RADIO->PREFIX0 = 0xE7;

  // Set logical address 0
  RADIO->TXADDRESS = 0;
  // Enable pipe 0
  RADIO->RXADDRESSES = 0x01;

  // Start receiver
  RADIO->TASKS_RXEN = 1;

  // Enable generating interrupts for END events
  RADIO->INTENSET = RADIO_INTENSET_END_Msk;

  // Enable handling of radio interrupts
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

  // Start transmitter
  RADIO->TASKS_TXEN = 1;

  // Enable generating interrupts for END events
  RADIO->INTENSET = RADIO_INTENSET_END_Msk;

  // Enable handling of radio interrupts
  NVIC_SETENA |= 1 << RADIO_IRQn;
}

void RADIO_IRQHandler() {}
