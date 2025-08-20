#ifndef RADIO_ARQ_H
#define RADIO_ARQ_H

#include <stddef.h>

void radio_arq_send(void *src, size_t src_len);
void radio_arq_receive(void *dest, size_t dest_len);

#endif  // RADIO_ARQ_H
