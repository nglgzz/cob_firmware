#ifndef USBD_HID_H
#define USBD_HID_H

#include <stdint.h>

void hid_send_report(uint32_t switches);

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t modifiers;
  uint8_t _reserved;
  uint8_t keys[6];
} hid_report_t;

extern hid_report_t hid_report;
extern uint32_t current_switches;

#endif  // USBD_HID_H
