
#include "radio_arq.h"

#include <stddef.h>
#include <stdint.h>

#include "probe.h"
#include "radio.h"
#include "timer.h"
#include "utils.h"

typedef enum {
  arq_packet_type_none,
  arq_packet_type_message,
  arq_packet_type_status,
} arq_packet_type_t;

#define RADIO_ARQ_PAYLOAD_MAXLEN (RADIO_PAYLOAD_MAXLEN - sizeof(arq_packet_type_t))

typedef struct {
  arq_packet_type_t type;
  uint8_t data[RADIO_ARQ_PAYLOAD_MAXLEN];
} __attribute__((packed)) arq_packet_t;

static inline void arq_ack() {
  arq_packet_t ack = {.type = arq_packet_type_status, .data = {1}};
  radio_send(&ack, sizeof(ack));
}

static inline void arq_nack() {
  arq_packet_t nack = {.type = arq_packet_type_status, .data = {0}};
  radio_send(&nack, sizeof(nack));
}

static bool radio_arq_busy = false;

void radio_arq_send(void *src, size_t src_len) {
  if (radio_arq_busy) return;
  radio_arq_busy = true;

  arq_packet_t packet = {.type = arq_packet_type_message, .data = {0}};
  arq_packet_t status = {.type = arq_packet_type_none, .data = {0}};

  memcpy((void *)packet.data, src, min(src_len, RADIO_ARQ_PAYLOAD_MAXLEN));

  int retries = 3;
  while (retries > 0) {
    int send_status = radio_send(&packet, sizeof(packet));
    probe_pulse_times(probe_tag_radio_payload, 3);
    probe_pulse_times(probe_tag_radio_payload, send_status + 2);

    int rec_status = radio_receive_timeout(&status, sizeof(status), 1e3);
    probe_pulse_times(probe_tag_radio_payload, 5);
    probe_pulse_times(probe_tag_radio_payload, rec_status + 2);
    probe_pulse_times(probe_tag_radio_payload, status.type + 2);
    probe_pulse_times(probe_tag_radio_payload, status.data[0] + 2);

    if (status.type == arq_packet_type_status && status.data[0] == 1) {
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

  arq_packet_t packet = {.type = arq_packet_type_none, .data = {0}};
  int status = radio_receive(&packet, sizeof(packet));

  probe_pulse_times(probe_tag_radio_payload, 1);
  probe_pulse_times(probe_tag_radio_payload, status + 2);
  if (status == 0) {
    arq_ack();
  } else {
    arq_nack();
  }

  memcpy(dest, packet.data, dest_len);
  radio_arq_busy = false;
}
