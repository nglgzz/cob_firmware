#include "radio.h"

#include <stdbool.h>
#include <stddef.h>

#include "clock.h"
#include "core.h"
#include "leds.h"
#include "nrf52840_bitfields.h"
#include "probe.h"
#include "timer.h"

radio_t *const RADIO = ((radio_t *)RADIO_BASE);

#define PAYLOAD_LEN 100
#define RADIO_FREQUENCY_CHANNEL 80  // 2480 MHz
#define RADIO_BASE_ADDRESS 0xE7E7E7E7

#define RADIO_DATA_WHITENING_IV 0x55
#define RADIO_CRC_INIT 0x0000AAAA
#define RADIO_CRC_POLY 0X12345678

#define RADIO_TXPOWER RADIO_TXPOWER_TXPOWER_0dBm
// Higher power is easier to notice in the power profiler
// #define RADIO_TXPOWER RADIO_TXPOWER_TXPOWER_Pos4dBm

typedef struct {
  volatile uint8_t len;
  volatile uint8_t data[RADIO_PAYLOAD_MAXLEN];
} radio_packet_t;

// Packet addresses to be used for the next transmission or reception. When
// transmitting, the packet pointed to by this address will be transmitted and
// when receiving, the received packet will be written to this address. This
// address is a byte aligned RAM address (must be in RAM).
static radio_packet_t tx_packet = {.len = PAYLOAD_LEN, .data = {0}};
static radio_packet_t rx_packet = {.len = PAYLOAD_LEN, .data = {0}};

void init_radio() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  // The radio is on by default, but doesn't hurt to be explicit.
  RADIO->POWER = 1;
  // Fixed on channel 80 for now, so 2480 MHz
  // TODO: figure out channel hopping
  RADIO->FREQUENCY = RADIO_FREQUENCY_CHANNEL;
  // https://en.wikipedia.org/wiki/DBm
  // 4dBm - 2.5mW - Bluetooth Class 2 radio, 10 m range
  // 0dBm - 1.0mW - Bluetooth standard (Class 3) radio, 1 m range
  RADIO->TXPOWER = RADIO_TXPOWER;

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
  RADIO->PCNF1 = (2 << RADIO_PCNF1_BALEN_Pos) |
                 (RADIO_PAYLOAD_MAXLEN << RADIO_PCNF1_MAXLEN_Pos) |
                 (0 << RADIO_PCNF1_STATLEN_Pos) |
                 (RADIO_PCNF1_WHITEEN_Enabled << RADIO_PCNF1_WHITEEN_Pos) |
                 (RADIO_PCNF1_ENDIAN_Little << RADIO_PCNF1_ENDIAN_Pos);

  // Data whitening initial value
  RADIO->DATAWHITEIV = RADIO_DATA_WHITENING_IV;

  // CRC is enabled and has length of 2 bytes
  RADIO->CRCCNF = RADIO_CRCCNF_LEN_Two << RADIO_CRCCNF_LEN_Pos;
  RADIO->CRCINIT = RADIO_CRC_INIT;
  RADIO->CRCPOLY = RADIO_CRC_POLY;

  // TX & RX: use logical address 0
  RADIO->BASE0 = RADIO_BASE_ADDRESS;
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
  NVIC_EnableIRQ(RADIO_IRQn);
}

static bool radio_busy = false;

/**
 * Return codes
 *    0 - Success
 *    1 - CRC error
 *    2 - RADIO busy
 *    3 - Timeout
 */
int radio_receive_timeout(void *dest, size_t dest_len, uint32_t timeout_us) {
  probe_on(probe_tag_radio_receive);

  if (radio_busy) {
    // This is to enforce receiving/sending serially as that's the only mode supported by the
    // RADIO peripheral.
    probe_off(probe_tag_radio_receive);
    return 2;
  }
  radio_busy = true;

  // Clear any pending events
  RADIO->EVENTS_READY = 0;
  RADIO->EVENTS_ADDRESS = 0;
  RADIO->EVENTS_PAYLOAD = 0;
  RADIO->EVENTS_END = 0;

  RADIO->PACKETPTR = (uint32_t)&rx_packet;

  probe_on(probe_tag_radio_rx);
  RADIO->TASKS_RXEN = 1;

  timer_start_timeout(TIMER1, timeout_us);
  while (!timer_has_timeout_expired(TIMER1) && RADIO->EVENTS_END == 0);

  if (RADIO->EVENTS_END) {
    RADIO->EVENTS_END = 0;
    radio_busy = false;
    probe_off(probe_tag_radio_rx);

    if (RADIO->CRCSTATUS == 1) {
      memcpy(dest,
             ((volatile radio_packet_t *)RADIO->PACKETPTR)->data,
             min(dest_len, rx_packet.len));
    }

    probe_off(probe_tag_radio_receive);
    return RADIO->CRCSTATUS == 1 ? 0 : 1;
  }

  radio_busy = false;
  probe_off(probe_tag_radio_rx);

  RADIO->TASKS_DISABLE = 1;
  probe_pulse_times(probe_tag_timeout, 5);  // timeout
  probe_off(probe_tag_radio_receive);
  return 3;
}

int radio_receive(void *dest, size_t dest_len) {
  return radio_receive_timeout(dest, dest_len, 0);
}

/**
 * Return codes
 *    0 - Success
 *    2 - RADIO busy
 *    3 - Timeout
 */
int radio_send(void *src, size_t src_len) {
  probe_on(probe_tag_radio_send);

  if (radio_busy) {
    // This is to enforce receiving/sending serially as that's the only mode supported by the
    // RADIO peripheral.
    probe_off(probe_tag_radio_send);
    return 2;
  }
  radio_busy = true;

  memcpy((void *)tx_packet.data, src, src_len);
  tx_packet.len = src_len;
  RADIO->PACKETPTR = (uint32_t)&tx_packet;

  // Clear events
  RADIO->EVENTS_READY = 0;
  RADIO->EVENTS_END = 0;

  probe_on(probe_tag_radio_tx);
  RADIO->TASKS_TXEN = 1;

  timer_start_timeout(TIMER1, 3e3);
  while (!timer_has_timeout_expired(TIMER1) && RADIO->EVENTS_END == 0);

  if (RADIO->EVENTS_END) {
    RADIO->EVENTS_END = 0;
    radio_busy = false;
    probe_off(probe_tag_radio_tx);

    probe_off(probe_tag_radio_send);
    return 0;
  }

  radio_busy = false;
  probe_off(probe_tag_radio_tx);

  RADIO->TASKS_DISABLE = 1;
  probe_pulse_times(probe_tag_timeout, 5);  // timeout
  probe_off(probe_tag_radio_send);
  return 3;
}
