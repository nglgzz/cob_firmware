#include "usbd_descriptors.h"

device_descriptor_t device_desc = {
    .bLength = sizeof(device_descriptor_t),
    .bDescriptorType = USBD_DESCRIPTOR_TYPE_Device,

    .bcdUSB = 0x0200,        // 2.0.0
    .bDeviceClass = 0,       // 0: each interface specifies its own class independently
    .bDeviceSubClass = 0,    // bDeviceClass is 0, this must also be 0
    .bDeviceProtocol = 0,    // same as above
    .bMaxPacketSize = 0x40,  // 64 bytes

    .idVendor = 0xFEED,
    .idProduct = 0x0000,
    .bcdDevice = 0x0001,  // 0.0.1

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01,
};

static const configuration_descriptor_t configuration_desc = {
    .bLength = sizeof(configuration_descriptor_t),
    .bDescriptorType = USBD_DESCRIPTOR_TYPE_Configuration,
    .wTotalLength = 0x0022,

    .bNumInterfaces = 0x01,
    .bConfigurationValue = 0x01,
    .iConfiguration = 0x04,

    .bmAttributes = 0xA0,  // Bus-powered, Remote Wakeup
    .bMaxPower = 0x32,     // 100 mA (0x32 = 50 dec)
};

static const interface_descriptor_t interface_desc = {
    .bLength = sizeof(interface_descriptor_t),
    .bDescriptorType = USBD_DESCRIPTOR_TYPE_Interface,

    .bInterfaceNumber = 0x00,
    .bAlternateSetting = 0x00,
    .bNumEndpoints = 0x01,

    .bInterfaceClass = 0x03,     // HID Class
    .bInterfaceSubClass = 0x00,  // 0: No subclass, 1: Boot interface
    .bInterfaceProtocol = 0x01,  // 0: None, 1: Keyboard, 2: Mouse
    .iInterface = 0x00,
};

static const uint8_t hid_desc[] = {
    0x09,                      // .bLength      sizeof(hid_class_descriptor_t)
    USBD_DESCRIPTOR_TYPE_HID,  // .bDescriptorType

    0x11,  // .bcdHID (L)             HID 1.11
    0x01,  // .bcdHID (H)             HID 1.11
    0x00,  // .bCountryCode       Not localized

    0x01,  // .bNumDescriptors

    // At least one descriptor must be specified, the following are optional.
    USBD_DESCRIPTOR_TYPE_HIDReport,  // .bDescriptorType
    0x3F,                            // .wDescriptorLength (L)   63 bytes
    0x00,                            // .wDescriptorLength (H)    63 bytes
};

static const endpoint_descriptor_t endpoint_desc = {
    .bLength = sizeof(endpoint_descriptor_t),
    .bDescriptorType = USBD_DESCRIPTOR_TYPE_Endpoint,

    .bEndpointAddress = 0x81,  // IN endpoint 1
    .bmAttributes = 0x03,      // Interrupt
    .wMaxPacketSize = 0x0008,  // 8 bytes
    .bInterval = 0x0A,         // 10ms
};

void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length, uint8_t _index) {
  *ptr = (uint8_t*)&device_desc;
  *length = sizeof(device_descriptor_t);
}

static uint8_t configuration0[64];
void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length, uint8_t _index) {
  USBD_DESCRIPTORS_Configuration(configuration0,
                                 *ptr,
                                 *length,
                                 DESCRIPTOR(configuration_desc),
                                 DESCRIPTOR(interface_desc),
                                 DESCRIPTOR(hid_desc),
                                 DESCRIPTOR(endpoint_desc));
}

static char* string_descriptors[] = {
    (char[]){0x09, 0x04},  // Lang ID: US English
    "nglgzz",              // Manufacturer
    "42 Dongle",           // Product name
    "0.0.1",               // Serial number
    "42 Keyboard"          // Interface name
};
void USBD_GetDescriptor_String(uint8_t** ptr, uint16_t* length, uint8_t index) {
  USBD_DESCRIPTORS_String(string_descriptors, ptr, length, index);
}

// --------------------------------------------
//        HID Report (Keyboard)
// --------------------------------------------
uint8_t hid_report_descriptor[] = {
    gUsagePage(1),      PAGE_GenericDesktop,     // Usage Page (Generic Desktop)
    lUsage(1),          USAGE_Keyboard,          // Usage (Keyboard)
    Collection(1),      COLLECTION_Application,  //   Collection (Application)

    gUsagePage(1),      PAGE_KeyboardKeypad,      //   MODIFIERS / Usage Page (Keyboard/Keypad)
    lUsageMinimum(1),   0xE0,                     //   Usage Minimum (224)
    lUsageMaximum(1),   0xE7,                     //   Usage Maximum (231)
    gLogicalMinimum(1), 0x00,                     //   Logical Minimum (0)
    gLogicalMaximum(1), 0x01,                     //   Logical Maximum (1)
    gReportSize(1),     0x01,                     //   Report Size (1)
    gReportCount(1),    0x08,                     //   Report Count (8)
    Input(1),           IO_DataVariableAbsolute,  //   Input (Data, Variable, Absolute)

    gReportCount(1),    0x01,              //   RESERVED / Report Count (1)
    gReportSize(1),     0x08,              //   Report Size (8)
    Input(1),           IO_ConstantValue,  //   Input (Constant)

    gReportCount(1),    0x05,                     //   LEDS / Report Count (5)
    gReportSize(1),     0x01,                     //   Report Size (1)
    gUsagePage(1),      0x08,                     //   Usage Page (LEDs)
    lUsageMinimum(1),   0x01,                     //   Usage Minimum (1)
    lUsageMaximum(1),   0x05,                     //   Usage Maximum (5)
    Output(1),          IO_DataVariableAbsolute,  //   Output (Data, Variable, Absolute)

    gReportCount(1),    0x01,              //   BYTE ALIGNING / Report Count (1)
    gReportSize(1),     0x03,              //   Report Size (3)
    Output(1),          IO_ConstantValue,  //   Output (Constant)

    gReportCount(1),    0x06,                 //   KEYS / Report Count (6)
    gReportSize(1),     0x08,                 //   Report Size (8)
    gLogicalMinimum(1), 0x00,                 //   Logical Minimum (0)
    gLogicalMaximum(1), 0x65,                 //   Logical Maximum (101)
    gUsagePage(1),      PAGE_KeyboardKeypad,  //   Usage Page (Keyboard/Keypad)
    lUsageMinimum(1),   0x00,                 //   Usage Minimum (0)
    lUsageMaximum(1),   0x65,                 //   Usage Maximum (101)
    Input(1),           IO_DataArray,         //   Input (Data, Array)

    EndCollection,  // End Collection
};

void USBD_GetDescriptor_HIDReport(uint8_t** ptr, uint16_t* length, uint8_t index) {
  *ptr = hid_report_descriptor;
  *length = sizeof(hid_report_descriptor);
}
