
#include "radio_arq.h"

#include <stddef.h>
#include <stdint.h>

#include "radio.h"
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
} arq_packet_t;

static inline void arq_ack() {
  arq_packet_t ack = {.type = arq_packet_type_status, .data = {1}};
  int status = radio_receive(&ack, sizeof(ack));
}

static inline void arq_nack() {
  arq_packet_t nack = {.type = arq_packet_type_status, .data = {0}};
  radio_send(&nack, sizeof(nack));
}

void radio_arq_send(void *src, size_t src_len) {
  arq_packet_t packet = {.type = arq_packet_type_message, .data = {0}};
  memcpy(&packet.data, src, min(src_len, RADIO_ARQ_PAYLOAD_MAXLEN));

  radio_send(&packet, sizeof(packet));
  arq_packet_t status = {.type = arq_packet_type_none, .data = {0}};

  while (status.type != arq_packet_type_status) {
    radio_receive(&status, sizeof(status));
    if (status.type == arq_packet_type_status) {
      if (status.data[0] == 1) {
        // ACK
        return;
      }

      if (status.data[0] == 0) {
        // NACK
        // TODO: add retry limit
        radio_arq_send(src, src_len);
      }
    } else {
      // Other message
      // TODO: add retry limit
      radio_arq_send(src, src_len);
    }
    delay(5000);
  }
}

void radio_arq_receive(void *dest, size_t dest_len) {
  arq_packet_t packet = {.type = arq_packet_type_none, .data = {0}};
  int status = radio_receive(&packet, sizeof(packet));

  if (status == 0) {
    arq_ack();
  } else {
    arq_nack();
    // TODO: add retry limit
    radio_arq_receive(dest, dest_len);
  }
}
