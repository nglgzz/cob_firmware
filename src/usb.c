#include "usbd.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

void USBD_Reset_Handler() {
  // EP0 & EP1
  USBD->EPINEN = 0x3;
}

device_descriptor_t device_desc = {
    .bLength = sizeof(device_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Device,

    .bcdUSB = 0x0200,        // 2.0.0
    .bDeviceClass = 0,       // 0: each interface specifies its own class independently
    .bDeviceSubClass = 0,    // bDeviceClass is 0, this must also be 0
    .bDeviceProtocol = 0,    // same as above
    .bMaxPacketSize = 0x40,  // 64 bytes

    .idVendor = 0xFEED,
    .idProduct = 0x0003,
    .bcdDevice = 0x0001,  // 0.0.1

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01,
};
void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length, uint8_t _index) {
  *ptr = (uint8_t*)&device_desc;
  *length = sizeof(device_descriptor_t);
}

static const configuration_descriptor_t configuration_desc = {
    .bLength = sizeof(configuration_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Configuration,
    .wTotalLength = 0x0000,  // This gets overwritten by USBD_DESC_CombineDescriptors (0x0022)

    .bNumInterfaces = 0x01,
    .bConfigurationValue = 0x01,
    .iConfiguration = 0x04,

    .bmAttributes = 0xA0,  // Bus-powered, Remote Wakeup
    .bMaxPower = 0x32,     // 100 mA (0x32 = 50 dec)
};

static const interface_descriptor_t interface_desc = {
    .bLength = sizeof(interface_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Interface,

    .bInterfaceNumber = 0x00,
    .bAlternateSetting = 0x00,
    .bNumEndpoints = 0x01,

    .bInterfaceClass = 0x03,  // HID Class
    // TODO: turn this into a boot subclass (and figure out what's needed to support that)
    .bInterfaceSubClass = 0x00,  // 0: No subclass, 1: Boot interface
    .bInterfaceProtocol = 0x01,  // 0: None, 1: Keyboard, 2: Mouse
    .iInterface = 0x00,
};

static const uint8_t hid_desc[] = {
    0x09,                // .bLength      sizeof(hid_class_descriptor_t)
    USBD_DESC_TYPE_HID,  // .bDescriptorType

    0x11,  // .bcdHID (L)             HID 1.11
    0x01,  // .bcdHID (H)             HID 1.11
    0x00,  // .bCountryCode       Not localized

    0x01,  // .bNumDescriptors

    // At least one descriptor must be specified, the following are optional.
    USBD_DESC_TYPE_HIDReport,  // .bDescriptorType
    47U,                       // .wDescriptorLength (L)   47 bytes
    0x00,                      // .wDescriptorLength (H)
};

static const endpoint_descriptor_t endpoint_desc = {
    .bLength = sizeof(endpoint_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Endpoint,

    .bEndpointAddress = 0x81,  // IN endpoint 1
    .bmAttributes = 0x03,      // Interrupt
    .wMaxPacketSize = 0x0008,  // 8 bytes
    .bInterval = 0x0A,         // 10ms
};
static uint8_t configuration0[64];
static descriptor_ptr_t descriptor_list[] = {USBD_DESC(configuration_desc),
                                             USBD_DESC(interface_desc),
                                             USBD_DESC(hid_desc),
                                             USBD_DESC(endpoint_desc)};

void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length, uint8_t _index) {
  uint16_t size = USBD_DESC_CombineDescriptors(
      configuration0, descriptor_list, sizeof(descriptor_list) / sizeof(descriptor_ptr_t));
  *ptr = configuration0;
  *length = size;
}

static char* string_descriptors[] = {
    (char[]){0x09, 0x04},  // Lang ID: US English
    "nglgzz",              // Manufacturer
    "42 Dongle",           // Product name
    "0.0.1",               // Serial number
    "42 Keyboard"          // Interface name
};
void USBD_GetDescriptor_String(uint8_t** ptr, uint16_t* length, uint8_t index) {
  USBD_DESC_GetString(string_descriptors, ptr, length, index);
}

void USBD_GetDescriptor_HIDReport(uint8_t** ptr, uint16_t* length, uint8_t index) {
  *ptr = hid_report_desc;
  *length = hid_report_desc_length;
}
