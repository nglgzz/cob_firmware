
#include "usbd_hid.h"

#include <stdint.h>

#include "usbd.h"

// --------------------------------------------
//        HID Report Descriptors
// --------------------------------------------

// HID 1.11
// 6.2.2.4 Main Items
#define Input(size) 0x80 | (0x03 & size)
#define Output(size) 0x90 | (0x03 & size)
#define Feature(size) 0xB0 | (0x03 & size)
#define Collection(size) 0xA0 | (0x03 & size)
#define EndCollection 0xC0

// HID 1.11
// 6.2.2.7 Global Items
#define gUsagePage(size) 0x04 | (0x03 & size)
#define gLogicalMinimum(size) 0x14 | (0x03 & size)
#define gLogicalMaximum(size) 0x24 | (0x03 & size)
#define gPhysicalMinimum(size) 0x34 | (0x03 & size)
#define gPhysicalMaximum(size) 0x44 | (0x03 & size)
#define gUnitExponent(size) 0x54 | (0x03 & size)
#define gUnit(size) 0x64 | (0x03 & size)
#define gReportSize(size) 0x74 | (0x03 & size)
#define gReportId(size) 0x84 | (0x03 & size)
#define gReportCount(size) 0x94 | (0x03 & size)
#define gReportPush(size) 0xA4 | (0x03 & size)
#define gReportPop(size) 0xB4 | (0x03 & size)

// HID 1.11
// 6.2.2.8 Local Items
#define lUsage(size) 0x08 | (0x03 & size)
#define lUsageMinimum(size) 0x18 | (0x03 & size)
#define lUsageMaximum(size) 0x28 | (0x03 & size)
#define lDesignatorIndex(size) 0x38 | (0x03 & size)
#define lDesignatorMinimum(size) 0x48 | (0x03 & size)
#define lDesignatorMaximum(size) 0x58 | (0x03 & size)
#define lStringIndex(size) 0x78 | (0x03 & size)
#define lStringMinimum(size) 0x88 | (0x03 & size)
#define lStringMaximum(size) 0x98 | (0x03 & size)
#define lDelimiter(size) 0xA8 | (0x03 & size)

// HID Usage Tables
// 3 Usage Pages
#define PAGE_GenericDesktop 0x01
#define PAGE_KeyboardKeypad 0x07
#define PAGE_Led 0x08
#define PAGE_Button 0x09
#define PAGE_Consumer 0x0C
#define PAGE_Digitizers 0x0D
#define PAGE_Unicode 0x10
#define PAGE_Sensors 0x20

// HID Usage Tables
// 4 Generic Desktop Page (0x01)
#define USAGE_Pointer 0x01
#define USAGE_Mouse 0x02
#define USAGE_Keyboard 0x06
#define USAGE_X 0x30
#define USAGE_Y 0x31
#define USAGE_Z 0x32
#define USAGE_Wheel 0x38
#define USAGE_MotionWakeup 0x3C

// HID 1.11
// 6.2.2.6 Collection, End Collection Items
//      A Collection item identifies a relationship between two or more data (Input,
//      Output, or Feature.) For example, a mouse could be described as a collection of
//      two to four data (x, y, button 1, button 2). While the Collection item opens a
//      collection of data, the End Collection item closes a collection.
#define COLLECTION_Application 0x01

#define IO_Data 0x00 << 0
#define IO_Constant 0x01 << 0
#define IO_Array 0x00 << 1
#define IO_Variable 0x01 << 1
#define IO_Absolute 0x00 << 2
#define IO_Relative 0x01 << 2

#define IO_DataArray IO_Data | IO_Array | IO_Absolute
#define IO_DataVariableRelative IO_Data | IO_Variable | IO_Relative
#define IO_DataVariableAbsolute IO_Data | IO_Variable | IO_Absolute
#define IO_ConstantValue IO_Constant | IO_Array | IO_Absolute

// --------------------------------------------
//        HID Report (Keyboard)
// --------------------------------------------
uint8_t hid_report_desc[] = {
    gUsagePage(1),      PAGE_GenericDesktop,     // Usage Page (Generic Desktop)
    lUsage(1),          USAGE_Keyboard,          // Usage (Keyboard)
    Collection(1),      COLLECTION_Application,  //   Collection (Application)

    gUsagePage(1),      PAGE_KeyboardKeypad,      //   MODIFIERS / Usage Page (Keyboard/Keypad)
    lUsageMinimum(1),   0xE0,                     //   Usage Minimum (224)
    lUsageMaximum(1),   0xE7,                     //   Usage Maximum (231)
    gLogicalMinimum(1), 0x00,                     //   Logical Minimum (0)
    gLogicalMaximum(1), 0x01,                     //   Logical Maximum (1)
    gReportCount(1),    0x08,                     //   Report Count (8)
    gReportSize(1),     0x01,                     //   Report Size (1)
    Input(1),           IO_DataVariableAbsolute,  //   Input (Data, Variable, Absolute)

    gReportCount(1),    0x01,              //   RESERVED / Report Count (1)
    gReportSize(1),     0x08,              //   Report Size (8)
    Input(1),           IO_ConstantValue,  //   Input (Constant, Array, Absolute)

    gUsagePage(1),      PAGE_KeyboardKeypad,  //   KEYS / Usage Page (Keyboard/Keypad)
    lUsageMinimum(1),   0x00,                 //   Usage Minimum (0)
    lUsageMaximum(1),   0xFF,                 //   Usage Maximum (255)
    gLogicalMinimum(1), 0x00,                 //   Logical Minimum (0)
    gLogicalMaximum(1), 0xFF,                 //   Logical Maximum (255)
    gReportCount(1),    0x06,                 //   Report Count (6)
    gReportSize(1),     0x08,                 //   Report Size (8)
    Input(1),           IO_DataArray,         //   Input (Data, Array, Absolute)

    EndCollection,  // End Collection
};
uint16_t hid_report_desc_length = sizeof(hid_report_desc);

// --------------------------------------------
//        HID Send Report
// --------------------------------------------

hid_report_keyboard_t hid_report_keyboard = {
    .modifiers = 0,
    ._reserved = 0,
    .keys = {0},
};

hid_report_mouse_t hid_report_mouse = {
    .report_id = 1,
    .x = 0,
    .y = 0,
};

uint8_t hid_keycodes[] = {0x00, 0x17, 0x15, 0x08};
uint32_t current_switches = 0;

void hid_send_report(uint32_t switches) {
  current_switches = switches;

  // HID report
  if (switches & 0x01)
    // Left shift
    hid_report_keyboard.modifiers = 0x02;
  else
    hid_report_keyboard.modifiers = 0x00;

  for (int i = 1; i < 4; i++) {
    if (switches & (0x01 << i))
      hid_report_keyboard.keys[i + 2] = hid_keycodes[i];
    else
      hid_report_keyboard.keys[i + 2] = 0x00;
  }

  usbd_state.transfer_in_progress = true;
  USBD->EPIN[1].PTR = (uint32_t)&hid_report_keyboard;
  USBD->EPIN[1].MAXCNT = sizeof(hid_report_keyboard);
  USBD->TASKS_STARTEPIN[1] = 1;
}
