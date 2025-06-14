#ifndef USBD_HID_H
#define USBD_HID_H

#include <stdint.h>

void hid_send_report(uint32_t switches);

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t modifiers;
  uint8_t _reserved;
  uint8_t keys[6];
} hid_report_keyboard_t;

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t report_id;
  uint16_t x;
  uint16_t y;
} hid_report_mouse_t;

extern hid_report_keyboard_t hid_report_keyboard;
extern hid_report_mouse_t hid_report_mouse;
extern uint32_t current_switches;

extern uint8_t hid_report_desc[];
extern uint16_t hid_report_desc_length;

#endif  // USBD_HID_H
