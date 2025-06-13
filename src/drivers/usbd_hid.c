
#include "usbd_hid.h"

#include <stdint.h>

#include "usbd.h"

hid_report_t hid_report = {
    .modifiers = 0,
    ._reserved = 0,
    .keys = {0},
};

uint8_t hid_keycodes[] = {0x00, 0x17, 0x15, 0x08};
uint32_t current_switches = 0;

void hid_send_report(uint32_t switches) {
  current_switches = switches;

  // HID report
  if (switches & 0x01)
    // Left shift
    hid_report.modifiers = 0x02;
  else
    hid_report.modifiers = 0x00;

  for (int i = 1; i < 4; i++) {
    if (switches & (0x01 << i))
      hid_report.keys[i + 2] = hid_keycodes[i];
    else
      hid_report.keys[i + 2] = 0x00;
  }

  usbd_state.transfer_in_progress = true;
  USBD->EPIN[1].PTR = (uint32_t)&hid_report;
  USBD->EPIN[1].MAXCNT = sizeof(hid_report);
  USBD->TASKS_STARTEPIN[1] = 1;
}
