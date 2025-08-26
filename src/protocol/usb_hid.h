#ifndef USBD_HID_H
#define USBD_HID_H

#include <stdint.h>

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t report_id;
  uint8_t modifiers;
  uint8_t _reserved;
  uint8_t keys[5];
} hid_report_keyboard_t;

typedef struct __attribute__((packed, aligned(4))) {
  uint8_t report_id;
  uint8_t buttons;
  int16_t x;
  int16_t y;
  int16_t wheel;
} hid_report_mouse_t;

extern hid_report_keyboard_t hid_report_keyboard;
extern hid_report_mouse_t hid_report_mouse;
extern uint32_t current_switches;

extern uint8_t hid_report_desc[];
extern uint16_t hid_report_desc_length;

void hid_send_report(uint32_t switches);
void hid_send_kb_report(hid_report_keyboard_t* report);

#endif  // USBD_HID_H
