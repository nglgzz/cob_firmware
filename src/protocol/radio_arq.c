
#include "radio_arq.h"

#include <stddef.h>
#include <stdint.h>

#include "probe.h"
#include "radio.h"
#include "timer.h"
#include "utils.h"

#define ARQ_PACKET_TYPE_NONE 0
#define ARQ_PACKET_TYPE_MESSAGE 1
#define ARQ_PACKET_TYPE_STATUS 2

#define RADIO_ARQ_PAYLOAD_MAXLEN 20

typedef struct {
  uint8_t type;
  uint8_t data[RADIO_ARQ_PAYLOAD_MAXLEN];
} __attribute__((packed)) arq_packet_t;

typedef struct {
  uint8_t type;
  uint8_t status;
  uint8_t reserved[2];
} __attribute__((packed)) arq_status_packet_t;

static void arq_ack() {
  arq_status_packet_t ack = {.type = ARQ_PACKET_TYPE_STATUS, .status = 1};
  radio_send(&ack, sizeof(arq_status_packet_t));
}

static void arq_nack() {
  arq_status_packet_t nack = {.type = ARQ_PACKET_TYPE_STATUS, .status = 0};
  radio_send(&nack, sizeof(arq_status_packet_t));
}

static bool radio_arq_busy = false;

void radio_arq_send(void *src, size_t src_len) {
  if (radio_arq_busy) return;
  radio_arq_busy = true;

  arq_packet_t packet = {.type = ARQ_PACKET_TYPE_MESSAGE, .data = {0}};
  arq_status_packet_t status = {.type = ARQ_PACKET_TYPE_NONE, .status = 0};

  memcpy((void *)packet.data, src, min(src_len, RADIO_ARQ_PAYLOAD_MAXLEN));

  int retries = 3;
  while (retries > 0) {
    // There can't be other operations happening in between sending and receiving, or the
    // sender will miss the acknowledgement packet.
    int send_status = radio_send(&packet, sizeof(packet));
    int rec_status = radio_receive_timeout(&status, sizeof(status), 1500);

    if (probes_initialized) {
      probe_pulse_times(probe_tag_radio_payload, send_status + 1);
      timer_sleep_us(90);
      probe_pulse_times(probe_tag_radio_payload, rec_status + 1);
      timer_sleep_us(90);
      probe_pulse_times(probe_tag_radio_payload, status.type + 1);
      timer_sleep_us(90);
      probe_pulse_times(probe_tag_radio_payload, status.status + 1);
    }

    if (status.type == ARQ_PACKET_TYPE_STATUS && status.status == 1) {
      // ACK
      radio_arq_busy = false;
      return;
    }

    retries--;
  }

  radio_arq_busy = false;
}

void radio_arq_receive(void *dest, size_t dest_len) {
  if (radio_arq_busy) return;
  radio_arq_busy = true;

  arq_packet_t packet = {.type = ARQ_PACKET_TYPE_NONE, .data = {0}};
  int status = radio_receive(&packet, sizeof(packet));
  if (status == 0) {
    arq_ack();
  } else {
    arq_nack();
  }

  memcpy(dest, packet.data, min(dest_len, RADIO_ARQ_PAYLOAD_MAXLEN));
  probe_pulse_times(probe_tag_radio_payload, 1 + status);
  radio_arq_busy = false;
}
