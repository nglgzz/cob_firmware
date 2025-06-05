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

configuration_descriptor_t configuration_desc = {
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

void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length) {
  *ptr = (uint8_t*)&device_desc;
  *length = sizeof(device_descriptor_t);
}

static uint8_t configuration0[64];
void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length) {
  USBD_DESCRIPTORS_Configuration(configuration0,
                                 DESCRIPTOR(configuration_desc),
                                 DESCRIPTOR(interface_desc),
                                 DESCRIPTOR(hid_desc),
                                 DESCRIPTOR(endpoint_desc));

  *ptr = configuration0;
  *length = (uint16_t)configuration0[2] | ((uint16_t)configuration0[3]) << 8;
}
