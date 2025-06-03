#ifndef USBD_DESCRIPTORS_H
#define USBD_DESCRIPTORS_H

#include <stdint.h>

// ----------------------
//        STATUS
// ----------------------
// USB Device Status
static const uint8_t device_status[] = {
    // D0: Self Powered (1 Self powered, 0 Bus powered)
    // D1: Remote Wakeup (1 Enable, 0 Disable)
    // D2-15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// USB Interface Status
static const uint8_t interface_status[] = {
    // D0-D15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// USB Endpoint Status
static const uint8_t endpoint_status[] = {
    // D0: Halt (1 Endpoint halted, 0 Endpoint not halted)
    // D1-D15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// -----------------------------
//        FEATURES
// -----------------------------
// https://www.engineersgarage.com/usb-requests-and-stages-of-control-transfer-part-4-6/
// Only remote wakeup (device feature) should be interesting

// -----------------------------
//        DESCRIPTORS
// -----------------------------

// --------------------------------------------
//    Device Descriptor
// --------------------------------------------

// Defined in the USB 2.0 specification
#define USBD_DESCRIPTOR_TYPE_Device 1
#define USBD_DESCRIPTOR_TYPE_Configuration 2
#define USBD_DESCRIPTOR_TYPE_String 3
#define USBD_DESCRIPTOR_TYPE_Interface 4
#define USBD_DESCRIPTOR_TYPE_Endpoint 5
#define USBD_DESCRIPTOR_TYPE_DeviceQualifier 6
#define USBD_DESCRIPTOR_TYPE_OtherSpeedConfiguration 7
#define USBD_DESCRIPTOR_TYPE_InterfacePower 8

// Defined in the USB Device Class Definition - HID
#define USBD_DESCRIPTOR_TYPE_HID 0x21
#define USBD_DESCRIPTOR_TYPE_HIDReport 0x22
#define USBD_DESCRIPTOR_TYPE_PhysicalDescriptor 0x23

static const uint8_t device_descriptor[] = {
    0x12,                         // bLength (18 bytes / size of this descriptor in bytes)
    USBD_DESCRIPTOR_TYPE_Device,  // bDescriptorType
    0x00,                         // bcdUSB L
    0x02,                         // bcdUSB H (USB 2.0 / JJ.M.N - J:major M:minor N:subminor)
    0x00,  // bDeviceClass (0: each interface specifies its own class independently)
    0x00,  // bDeviceSubClass (if bDeviceClass is 0, this must also be 0)
    0x00,  // bDeviceProtocol (same as above)
    0x40,  // bMaxPacketSize0 (64 bytes)
    // Temporary vendor and product IDs, once the repo is open sourced, request
    // vid & pid here:
    // https://pid.codes/howto/
    0x5E,  // # idVendor L
    0x04,  // # idVendor H (Example: Nordic Semiconductor)
    0x23,  // # idProduct L
    0x57,  // # idProduct H (Example: Custom Keyboard)
    0x01,  // # bcdDevice L
    0x00,  // # bcdDevice H (0.0.1 / JJ.M.N - J:major M:minor N:subminor)
    0x01,  // iManufacturer (index of string descriptor describing manufacturer)
    0x02,  // iProduct (index of string descriptor describing product)
    0x03,  // iSerialNumber (index of string descriptor describing serialnumber)
    0x01   // bNumConfigurations (number of possible configurations)
};

// --------------------------------------------
//    Configuration Descriptor
// --------------------------------------------

// USB Configuration Descriptor (with HID Descriptor for a keyboard)
static const uint8_t configuration_descriptor[] = {
    // Configuration descriptor
    0x09,                                // bLength
    USBD_DESCRIPTOR_TYPE_Configuration,  // bDescriptorType
    0x22,                                // # wTotalLength L
    0x00,  // # wTotalLength H (34 bytes / includes combined length of all descriptors under
           // # configuration (configuration, interface, endpoint, and class/vendor specific))
    0x01,  // # bNumInterfaces
    0x01,  // bConfigurationValue (value to use as an argument of SetConfiguration to select
           // this configuration)
    0x04,  // iConfiguration (index of string descriptor describing this configuration)
    // D0-D4: Reserved (reset to zero)
    // D5: Remote Wakeup
    // D6: 0 Bus-powered, 1 Self-powered
    // D7: Reserved (set to 1)
    0xA0,  // bmAttributes (Bus-powered, Remote Wakeup)
    0x32,  // # bMaxPower (100 mA / unit is 2mA)

    // Interface descriptor
    0x09,                            // bLength
    USBD_DESCRIPTOR_TYPE_Interface,  // bDescriptorType
    0x00,                            // bInterfaceNumber
    0x00,  // bAlternateSetting (value used to select this alternate setting for the interface
           // identified in the prior field)
    0x01,  // bNumEndpoints (number of endpoints used by this device, excluding endpoint zero)
    0x03,  // bInterfaceClass (3: HID Class)
    // The HID Class doesn't use subclasses to define most protocols, instead the data protocol
    // and data provided is defined in the report descriptor. Since the HID report descriptor
    // parsing is a significant amount of code, which is not available during BIOS, a subclass
    // of Boot Interface is made available for devices that need BIOS support.
    0x01,  // bInterfaceSubClass (0: No Subclass, 1: Boot Interface)
    // bInterfaceProtocol only has meaning if bInterfaceSubClass declares that the device
    // supports a boot interface, otherwise it is 0.
    0x01,  // bInterfaceProtocol (0: None, 1: Keyboard, 2: Mouse)
    0x00,  // iInterface (index of string descriptor describing this interface)

    // HID descriptor
    0x09,                      // bLength
    USBD_DESCRIPTOR_TYPE_HID,  // bDescriptorType
    0x11,                      // bcdHID L
    0x01,                      // bcdHID H (HID 1.11 / HID Class Specification release)
    // If the hardware is localized, this would define which country it is localized for. Zero
    // means the hardware is not localized.
    0x00,  // bCountryCode
    // Number of class descriptors, must be at least 1 (i.e. Report descriptor).
    0x01,                            // # bNumDescriptors
    USBD_DESCRIPTOR_TYPE_HIDReport,  // # bDescriptorType[0]
    0x3F,                            // # wDescriptorLength[0] L
    0x00,                            // # wDescriptorLength[0] H (63 bytes)
    // Additional class descriptors can be added here, by adding the corresponding
    // bDescriptorType and wDescriptorLength values.

    // Endpoint descriptor
    0x07,                           // bLength
    USBD_DESCRIPTOR_TYPE_Endpoint,  // bDescriptorType
    // D0-D3: Endpoint number
    // D4-D6: Reserved, set to 0
    // D7: Direction, ignored for control endpoints (0 OUT endpoint, 1 IN endpoint)
    0x81,  // bEndpointAddress (IN endpoint 1)
    // D0-D1: (0 Control, 1 Isochronous, 2 Bulk, 3 Interrupt)
    // If an isochronous endpoint:
    //    D2-D3: Synchronization type (0 no sync, 1 async, 2 adaptive, 3 sync)
    //    D4-D5: Usage type (0 data, 1 feedback, 2 implicit feedback data, 3 reserved)
    //    D6-D7: Reserved, set to 0
    // Otherwise all other bits are reserved, set to 0.
    0x03,  // bmAttributes (Interrupt)
    // Max packet size that this endpoint is capable of receiving or sending.
    // D0-D10: Max size in bytes
    // D11-D12: Only relevant in high-speed isochronous and interrupt endpoints.
    // D13-D15: Reserved, set to 0
    0x08,  // wMaxPacketSize L
    0x00,  // wMaxPacketSize H (8 bytes)
    // Interval for polling the endpoint for data transfers in milliseconds (assuming
    // full-speed). For full-/low-speed interrupt endpoints, the value of this field may be
    // from 1 to 255.
    0x0A,  // # bInterval (10 ms)
};

// --------------------------------------------
//    String Descriptors
// --------------------------------------------

const uint8_t string_descriptor_0_langID[] = {
    0x04,                         // bLength
    USBD_DESCRIPTOR_TYPE_String,  // bDescriptorType
    0x09,
    0x04  // wLANGID[0] = 0x0409 (US English)
};

static const uint8_t string_descriptor_4_interface[] = {
    0x18,                         // bLength (2 + 6 characters * 2 / UTF-16LE encoded)
    USBD_DESCRIPTOR_TYPE_String,  // bDescriptorType
    '4',
    0,
    '2',
    0,
    ' ',
    0,
    'K',
    0,
    'e',
    0,
    'y',
    0,
    'b',
    0,
    'o',
    0,
    'a',
    0,
    'r',
    0,
    'd',
    0,
};

static const uint8_t string_descriptor_1_manufacturer[] = {
    0x0E,                         // bLength (2 + 6 characters * 2 / UTF-16LE encoded)
    USBD_DESCRIPTOR_TYPE_String,  // bDescriptorType
    'n',
    0,
    'g',
    0,
    'l',
    0,
    'g',
    0,
    'z',
    0,
    'z',
    0,
};

static const uint8_t string_descriptor_2_product[] = {
    0x04,                         // bLength (2 + 6 characters * 2 / UTF-16LE encoded)
    USBD_DESCRIPTOR_TYPE_String,  // bDescriptorType
    '4',
    0,
    '2',
    0,
};

static const uint8_t string_descriptor_3_serial_number[] = {
    0x0E,                         // bLength (2 + 6 characters * 2 / UTF-16LE encoded)
    USBD_DESCRIPTOR_TYPE_String,  // bDescriptorType
    '0',
    0,
    '.',
    0,
    '0',
    0,
    '.',
    0,
    '0',
    0,
    '1',
    0,
};

// --------------------------------------------
//    HID Report Descriptor
// --------------------------------------------

// Main items
#define Input(size) 0x80 | (0x03 & size)
#define Output(size) 0x90 | (0x03 & size)
#define Feature(size) 0xB0 | (0x03 & size)
#define Collection(size) 0xA0 | (0x03 & size)
#define EndCollection 0xC0

// Global Items
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

// Local Items
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

// Pages
#define PAGE_GenericDesktop 0x01
#define PAGE_KeyboardKeypad 0x07
#define PAGE_Led 0x08
#define PAGE_Button 0x09
#define PAGE_Consumer 0x0C
#define PAGE_Digitizers 0x0D
#define PAGE_Unicode 0x10
#define PAGE_Sensors 0x20

// Usages
#define USAGE_Keyboard 0x06

// Collections
#define COLLECTION_Application 0x01

#define IO_Data 0x00 << 0
#define IO_Constant 0x01 << 0
#define IO_Array 0x00 << 1
#define IO_Variable 0x01 << 1
#define IO_Absolute 0x00 << 2
#define IO_Relative 0x01 << 2

#define IO_DataArray IO_Data | IO_Array
#define IO_DataVariableAbsolute IO_Data | IO_Variable | IO_Absolute
#define IO_ConstantValue IO_Constant | IO_Variable

// HID Report Descriptor (Keyboard)
static const uint8_t hid_report_descriptor[] = {
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

    EndCollection  // End Collection
};

#endif  // USBD_DESCRIPTORS_H
