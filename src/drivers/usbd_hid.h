#ifndef USBD_HID_H
#define USBD_HID_H

#include <stdint.h>

void usbd_hid_send(uint32_t switches);

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t modifiers;
  uint8_t _reserved;
  uint8_t keys[6];
} usbd_hid_report_t;

extern usbd_hid_report_t usbd_hid_report;
extern uint32_t current_switches;

#endif  // USBD_HID_H
