
#include "usb_hid.h"

#include <stdint.h>

#include "usbd.h"

// --------------------------------------------
//        HID Report Descriptors
// --------------------------------------------

// HID 1.11
// 6.2.2.4 Main Items
#define Input(size, value) 0x80 | (0x03 & size), value
#define Output(size, value) 0x90 | (0x03 & size), value
#define Feature(size, value) 0xB0 | (0x03 & size), value
#define Collection(size, value) 0xA0 | (0x03 & size), value
#define EndCollection 0xC0

// HID 1.11
// 6.2.2.7 Global Items
#define UsagePage(size, value) 0x04 | (0x03 & size), value
#define LogicalMinimum(size, value) 0x14 | (0x03 & size), value
#define LogicalMaximum(size, value) 0x24 | (0x03 & size), value
#define PhysicalMinimum(size, value) 0x34 | (0x03 & size), value
#define PhysicalMaximum(size, value) 0x44 | (0x03 & size), value
#define UnitExponent(size, value) 0x54 | (0x03 & size), value
#define Unit(size, value) 0x64 | (0x03 & size), value
#define ReportSize(size, value) 0x74 | (0x03 & size), value
#define ReportId(size, value) 0x84 | (0x03 & size), value
#define ReportCount(size, value) 0x94 | (0x03 & size), value
#define ReportPush(size, value) 0xA4 | (0x03 & size), value
#define ReportPop(size, value) 0xB4 | (0x03 & size), value

// HID 1.11
// 6.2.2.8 Local Items
#define Usage(size, value) 0x08 | (0x03 & size), value
#define UsageMinimum(size, value) 0x18 | (0x03 & size), value
#define UsageMaximum(size, value) 0x28 | (0x03 & size), value
#define DesignatorIndex(size, value) 0x38 | (0x03 & size), value
#define DesignatorMinimum(size, value) 0x48 | (0x03 & size), value
#define DesignatorMaximum(size, value) 0x58 | (0x03 & size), value
#define StringIndex(size, value) 0x78 | (0x03 & size), value
#define StringMinimum(size, value) 0x88 | (0x03 & size), value
#define StringMaximum(size, value) 0x98 | (0x03 & size), value
#define Delimiter(size, value) 0xA8 | (0x03 & size), value

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
// (group of axes)
#define COLLECTION_Physical 0x00
// (mouse, keyboard)
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
    // KEYBOARD
    UsagePage(1, PAGE_GenericDesktop),
    Usage(1, USAGE_Keyboard),
    Collection(1, COLLECTION_Application),
    //    REPORT ID (1 byte)
    ReportId(1, 0x01),

    //    MODIFIERS (1 byte)
    UsagePage(1, PAGE_KeyboardKeypad),
    UsageMinimum(1, 0xE0),
    UsageMaximum(1, 0xE7),
    LogicalMinimum(1, 0x00),
    LogicalMaximum(1, 0x01),
    ReportSize(1, 0x01),
    ReportCount(1, 0x08),
    Input(1, IO_DataVariableAbsolute),

    //    RESERVED (1 byte)
    ReportSize(1, 0x08),
    ReportCount(1, 0x01),
    Input(1, IO_ConstantValue),

    //    KEYS (5 bytes)
    UsagePage(1, PAGE_KeyboardKeypad),
    UsageMinimum(1, 0x00),
    UsageMaximum(1, 0xFF),
    LogicalMinimum(1, 0x00),
    LogicalMaximum(1, 0xFF),
    ReportSize(1, 0x08),
    ReportCount(1, 0x05),
    Input(1, IO_DataArray),
    EndCollection,  // (Application)
    // END KEYBOARD

    // MOUSE
    UsagePage(1, PAGE_GenericDesktop),
    Usage(1, USAGE_Mouse),
    Collection(1, COLLECTION_Application),
    //    REPORT ID (1 byte)
    ReportId(1, 0x02),
    Usage(1, USAGE_Pointer),
    Collection(1, COLLECTION_Physical),

    //    BUTTONS (1 byte)
    UsagePage(1, PAGE_Button),
    UsageMinimum(1, 0x01),
    UsageMaximum(1, 0x08),
    LogicalMinimum(1, 0x00),
    LogicalMaximum(1, 0x01),
    ReportCount(1, 0x03),
    ReportSize(1, 0x01),
    Input(1, IO_DataVariableAbsolute),

    //    X, Y, WHEEL (6 bytes)
    UsagePage(1, PAGE_GenericDesktop),
    Usage(1, USAGE_X),
    Usage(1, USAGE_Y),
    Usage(1, USAGE_Wheel),
    LogicalMinimum(1, 0xFF),  // 255
    LogicalMaximum(1, 0x01),  // -255
    ReportSize(1, 0x0F),      // 16 bits
    ReportCount(1, 0x03),
    Input(1, IO_DataVariableRelative),

    EndCollection,  // (Physical)
    EndCollection,  // (Application)
};
uint16_t hid_report_desc_length = sizeof(hid_report_desc);

// --------------------------------------------
//        HID Send Report
// --------------------------------------------

hid_report_keyboard_t hid_report_keyboard = {
    .report_id = 1,
    .modifiers = 0,
    ._reserved = 0,
    .keys = {0},
};

hid_report_mouse_t hid_report_mouse = {
    .report_id = 2,
    .buttons = 0,
    .x = 0,
    .y = 0,
    .wheel = 0,
};

uint8_t hid_keycodes[] = {0x00, 0x17, 0x15, 0x08};
uint32_t current_switches = 0;

void hid_send_report(uint32_t switches) {
  if (!(usbd_state.usbd_ready && usbd_state.vbus_detected && usbd_state.usb_power_ready)) {
    return;
  }

  current_switches = switches;
  bool keyboard_changed = (switches & 0x0C) || hid_report_keyboard.keys[0] ||
                          hid_report_keyboard.keys[1] || hid_report_keyboard.keys[2];

  // HID report
  if (switches & 0x01)
    // Left shift
    hid_report_mouse.x += 200;
  else if (switches & 0x02)
    hid_report_mouse.x -= 200;
  else
    hid_report_mouse.x = 0;

  for (int i = 2; i < 4; i++) {
    if (switches & (0x01 << i))
      hid_report_keyboard.keys[i - 1] = hid_keycodes[i];
    else
      hid_report_keyboard.keys[i - 1] = 0x00;
  }

  // TODO: implement queue or split into separate endpoints
  if (keyboard_changed) {
    usbd_epin_start(1, (uint32_t)&hid_report_keyboard, sizeof(hid_report_keyboard));
  } else {
    usbd_epin_start(1, (uint32_t)&hid_report_mouse, sizeof(hid_report_mouse));
  }
}
