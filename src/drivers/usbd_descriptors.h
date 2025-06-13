#ifndef USBD_DESCRIPTORS_H
#define USBD_DESCRIPTORS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint8_t* ptr;
  uint16_t size;
} descriptor_ptr_t;

uint16_t usbd_descriptors_set_configuration_descriptor(uint8_t combined_descriptor[],
                                                       descriptor_ptr_t descriptor_list[],
                                                       size_t length);

// Creates descriptor pointer from a struct
#define DESCRIPTOR(desc) {.ptr = (uint8_t*)&desc, .size = sizeof(desc)}

// Takes an array of bytes and a variable number of descriptors, and copies the descriptors
// into the byte array.
#define USBD_DESCRIPTORS_Configuration(combined_desc, ptr, length, ...)                      \
  descriptor_ptr_t desc_list[] = {__VA_ARGS__};                                              \
  size_t desc_list_length = sizeof(desc_list) / sizeof(descriptor_ptr_t);                    \
  usbd_descriptors_set_configuration_descriptor(combined_desc, desc_list, desc_list_length); \
  ptr = configuration0;                                                                      \
  length = (uint16_t)configuration0[2] | ((uint16_t)configuration0[3]) << 8

void USBD_DESCRIPTORS_String(char* string_descriptors[], uint8_t** ptr, uint16_t* length,
                             uint8_t index);

// USB Request handlers
void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_String(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_HIDReport(uint8_t** ptr, uint16_t* length, uint8_t index);

// -----------------------------
//        FEATURES
// -----------------------------
// https://www.engineersgarage.com/usb-requests-and-stages-of-control-transfer-part-4-6/
// Only remote wakeup (device feature) should be interesting

// -----------------------------
//        DESCRIPTORS
// -----------------------------
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

// Universal Serial Bus Specification Revision 2.0
// Table 9-8. Standard DEVICE Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bLength;          // Size of this descriptor in bytes
  uint8_t bDescriptorType;  // Descriptor type constant
  uint16_t bcdUSB;  // Binary coded decimal representing the supported USB version (JJ.M.N -
                    // J:major M:minor N:subminor)
  uint8_t bDeviceClass;     // Zero to let each interface specify its own class
  uint8_t bDeviceSubClass;  // Must be set to zero if bDeviceClass is zero
  uint8_t bDeviceProtocol;  // Must be set to zero if bDeviceClass is zero
  uint8_t bMaxPacketSize;   // Maximum packet size for endpoint 0

  // A vendor and product ID can be requested for open source projects here:
  // https://pid.codes/howto/
  uint16_t idVendor;
  uint16_t idProduct;
  uint16_t
      bcdDevice;  // Binary coded decimal device version (JJ.M.N - J:major M:minor N:subminor)

  uint8_t iManufacturer;  // Index of string descriptor describing the manufacturer
  uint8_t iProduct;       // Index of string descriptor describing the product
  uint8_t iSerialNumber;  // Index of string descriptor describing the serial number

  uint8_t bNumConfigurations;  // Number of possible configurations
} device_descriptor_t;

// Universal Serial Bus Specification Revision 2.0
// Table 9-10. Standard CONFIGURATION Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bLength;          // Size of this descriptor in bytes
  uint8_t bDescriptorType;  // Descriptor type constant
  uint16_t wTotalLength;    // Combined length of all descriptors under configuration
                            // (configuration, interface, endpoint, and class/vendor specific)
  uint8_t bNumInterfaces;
  uint8_t bConfigurationValue;  // Value to use as an argument of SetConfiguration to select
                                // this configuration
  uint8_t iConfiguration;       // Index of string descriptor for this configuration

  // D0-D4: Reserved (reset to zero)
  // D5: Remote Wakeup
  // D6: 0 Bus-powered, 1 Self-powered
  // D7: Reserved (set to 1)
  uint8_t bmAttributes;
  uint8_t bMaxPower;  // The unit is 2mA, (e.g. 0x32 (50dec) is 100mA)
} configuration_descriptor_t;

// Universal Serial Bus Specification Revision 2.0
// Table 9-12. Standard INTERFACE Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bLength;            // Size of this descriptor in bytes
  uint8_t bDescriptorType;    // Descriptor type constant
  uint8_t bInterfaceNumber;   // Zero-based value identifying this interface in the array of
                              // concurrent interfaces supported by the current configuration.
  uint8_t bAlternateSetting;  // Value used to select this alternate setting for the interface
                              // identified in the prior field (no idea what this means)
  uint8_t bNumEndpoints;      // Number of endpoints used by this interface.

  uint8_t bInterfaceClass;     // Class code
  uint8_t bInterfaceSubClass;  // Subclass code
  uint8_t bInterfaceProtocol;  // Protocol code
  uint8_t iInterface;          // Index of string descriptor describing this interface
} interface_descriptor_t;

// Universal Serial Bus Specification Revision 2.0
// Table 9-13. Standard ENDPOINT Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bLength;          // Size of this descriptor in bytes
  uint8_t bDescriptorType;  // Descriptor type constant

  // D0-D3: Endpoint number
  // D4-D6: Reserved, set to 0
  // D7: Direction, ignored for control endpoints (0 OUT endpoint, 1 IN endpoint)
  uint8_t bEndpointAddress;

  // D0-D1: (0 Control, 1 Isochronous, 2 Bulk, 3 Interrupt)
  // If an isochronous endpoint:
  //    D2-D3: Synchronization type (0 no sync, 1 async, 2 adaptive, 3 sync)
  //    D4-D5: Usage type (0 data, 1 feedback, 2 implicit feedback data, 3 reserved)
  //    D6-D7: Reserved, set to 0
  // Otherwise all other bits are reserved, set to 0.
  uint8_t bmAttributes;

  // Max packet size that this endpoint is capable of receiving or sending.
  // D0-D10: Max size in bytes
  // D11-D12: Only relevant in high-speed isochronous and interrupt endpoints.
  // D13-D15: Reserved, set to 0
  uint16_t wMaxPacketSize;

  // Interval for polling the endpoint for data transfers in milliseconds (assuming
  // full-speed).
  //
  // For full-/low-speed interrupt endpoints, the value of this field may be from 1 to 255.
  //
  // For full-/high-speed isochronous endpoints, this value must be in the range from 1 to 16.
  // The bInterval value is used as the exponent for a 2^(bInterval-1) value; e.g., a bInterval
  // of 4 means a period of 8 (2^(4-1)).
  uint8_t bInterval;
} endpoint_descriptor_t;

// Device Class Definition for Human Interface Devices (HID) Version 1.11
// 6.2.1 HID Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bDescriptorType;     // Constant name identifying type of class descriptor. See
                               // Section 7.1.2: Set_Descriptor Request for a table of class
                               // descriptor constants.
  uint16_t wDescriptorLength;  // Size of the Report descriptor.
} hid_class_descriptor_t;

// Device Class Definition for Human Interface Devices (HID) Version 1.11
// 6.2.1 HID Descriptor
typedef struct __attribute__((packed)) {
  uint8_t bLength;          // Size of this descriptor in bytes
  uint8_t bDescriptorType;  // Descriptor type constant

  uint16_t bcdHID;  // Binary coded decimal representing the supported USB version (JJ.M.N -
                    // J:major M:minor N:subminor)

  uint8_t bCountryCode;  // If the hardware is localized, this would define which country it is
                         // localized for. Zero means the hardware is not localized.

  uint8_t bNumDescriptors;  // Number of class descriptors (always at least one i.e. Report
                            // descriptor.)

  // At least one descriptor must be specified, the following are optional.
  hid_class_descriptor_t hidClassDescriptors[];
} hid_descriptor_t;

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

#endif  // USBD_DESCRIPTORS_H
