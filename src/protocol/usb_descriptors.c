
#include "usb_descriptors.h"

#include "usb_hid.h"
#include "usbd.h"
#include "utils.h"

// Creates descriptor pointer from a struct
#define USBD_DESC(desc) {.ptr = (uint8_t *)&desc, .size = sizeof(desc)}

uint16_t USBD_DESC_CombineDescriptors(uint8_t combined_descriptor[],
                                      descriptor_ptr_t descriptor_list[], size_t length);
void USBD_DESC_GetString(char *string_descriptors[], uint8_t **ptr, uint16_t *length,
                         uint8_t index);

// ----------------------
//        STATUS
// ----------------------
// TODO: do I need this?
// USB Device Status
static uint8_t device_status[] = {
    // D0: Self Powered (1 Self powered, 0 Bus powered)
    // D1: Remote Wakeup (1 Enable, 0 Disable)
    // D2-15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// USB Interface Status
static uint8_t interface_status[] = {
    // D0-D15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// USB Endpoint Status
static uint8_t endpoint_status[] = {
    // D0: Halt (1 Endpoint halted, 0 Endpoint not halted)
    // D1-D15: Reserved (Must be set to 0)
    0x00,
    0x00,
};

// --------------------------------------------
//        DEVICE DESCRIPTOR
// --------------------------------------------
device_descriptor_t device_desc = {
    .bLength = sizeof(device_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Device,

    .bcdUSB = 0x0200,        // 2.0.0
    .bDeviceClass = 0,       // 0: each interface specifies its own class independently
    .bDeviceSubClass = 0,    // bDeviceClass is 0, this must also be 0
    .bDeviceProtocol = 0,    // same as above
    .bMaxPacketSize = 0x40,  // 64 bytes

#ifdef USB_VENDOR_ID
    .idVendor = USB_VENDOR_ID,
#else
    .idVendor = 0xFEED,
#endif

#ifdef USB_PRODUCT_ID
    .idProduct = USB_PRODUCT_ID,
#else
    .idProduct = 0x0003,
#endif

#ifdef USB_DEVICE_VERSION
    .bcdDevice = USB_DEVICE_VERSION,
#else
    .bcdDevice = 0x0001,  // 0.0.1
#endif

    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,

    .bNumConfigurations = 0x01,
};

// --------------------------------------------
//        CONFIGURATION 0 DESCRIPTOR
// --------------------------------------------
static const configuration_descriptor_t configuration0_desc = {
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
    .bInterfaceProtocol = 0x00,  // 0: None, 1: Keyboard, 2: Mouse
    .iInterface = 0x00,
};

static const uint8_t hid_desc[] = {
    // TODO: create struct for HID descriptor
    0x09,                // .bLength      sizeof(hid_class_descriptor_t)
    USBD_DESC_TYPE_HID,  // .bDescriptorType

    0x11,  // .bcdHID (L)             HID 1.11
    0x01,  // .bcdHID (H)             HID 1.11
    0x00,  // .bCountryCode       Not localized

    0x01,  // .bNumDescriptors

    // At least one descriptor must be specified, the following are optional.
    USBD_DESC_TYPE_HIDReport,  // .bDescriptorType
    // TODO: can I make this update automatically?
    95U,   // .wDescriptorLength (L) in bytes
    0x00,  // .wDescriptorLength (H)
};

static const endpoint_descriptor_t endpoint_desc = {
    .bLength = sizeof(endpoint_descriptor_t),
    .bDescriptorType = USBD_DESC_TYPE_Endpoint,

    .bEndpointAddress = 0x81,  // IN endpoint 1
    .bmAttributes = 0x03,      // Interrupt
    .wMaxPacketSize = 0x0016,  // 16 bytes
    .bInterval = 0x0A,         // 10ms
};

static uint8_t configuration0[64];
static descriptor_ptr_t configuration0_desc_list[] = {USBD_DESC(configuration0_desc),
                                                      USBD_DESC(interface_desc),
                                                      USBD_DESC(hid_desc),
                                                      USBD_DESC(endpoint_desc)};

// --------------------------------------------
//        STRING DESCRIPTORS
// --------------------------------------------
static char *string_descriptors[] = {
    (char[]){0x09, 0x04},  // Lang ID: US English
    "nglgzz",              // Manufacturer
    "42 Dongle",           // Product name
    "0.0.1",               // Serial number
    "42 Keyboard"          // Interface name
};

// --------------------------------------------
//        DESCRIPTOR HANDLERS
// --------------------------------------------
void USBD_Reset_Handler() {
  // EP0 & EP1
  USBD->EPINEN = 0x3;
}

void USBD_GetDescriptor_Device(uint8_t **ptr, uint16_t *length, uint8_t _index) {
  *ptr = (uint8_t *)&device_desc;
  *length = sizeof(device_descriptor_t);
}

void USBD_GetDescriptor_Configuration(uint8_t **ptr, uint16_t *length, uint8_t _index) {
  uint16_t size = USBD_DESC_CombineDescriptors(
      configuration0,
      configuration0_desc_list,
      sizeof(configuration0_desc_list) / sizeof(descriptor_ptr_t));
  *ptr = configuration0;
  *length = size;
}

void USBD_GetDescriptor_String(uint8_t **ptr, uint16_t *length, uint8_t index) {
  USBD_DESC_GetString(string_descriptors, ptr, length, index);
}

void USBD_GetDescriptor_HIDReport(uint8_t **ptr, uint16_t *length, uint8_t index) {
  *ptr = hid_report_desc;
  *length = hid_report_desc_length;
}

// --------------------------------------------
//        DESCRIPTOR HELPERS
// --------------------------------------------
uint16_t USBD_DESC_CombineDescriptors(uint8_t combined_descriptor[],
                                      descriptor_ptr_t descriptor_list[], size_t length) {
  uint8_t *write_ptr = combined_descriptor;
  uint16_t total_size = 0;

  for (int i = 0; i < length; i++) {
    memcpy(write_ptr, descriptor_list[i].ptr, descriptor_list[i].size);
    total_size += descriptor_list[i].size;
    write_ptr += descriptor_list[i].size;
  }

  // Update combined total size of descriptor
  combined_descriptor[2] = (uint8_t)(total_size & 0xFF);
  combined_descriptor[3] = (uint8_t)(total_size >> 8);

  return total_size;
}

static uint8_t _string_descriptor[128];
void USBD_DESC_GetString(char *string_descriptors[], uint8_t **ptr, uint16_t *length,
                         uint8_t index) {
  if (index == 0) {
    // LANGID descriptor
    _string_descriptor[0] = 0x04;                   // bLength
    _string_descriptor[1] = USBD_DESC_TYPE_String;  // bDescriptorType
    _string_descriptor[2] = string_descriptors[0][0];
    _string_descriptor[3] = string_descriptors[0][1];

    *ptr = (uint8_t *)_string_descriptor;
    *length = (uint16_t)0x0004;
    return;
  }

  if (index > 4) {
    return;
  }

  for (int i = 0; i < 32; i++) {
    if (string_descriptors[index][i] == '\0') {
      _string_descriptor[0] = (i + 1) * 2;            // bLength
      _string_descriptor[1] = USBD_DESC_TYPE_String;  // bDescriptorType
      break;
    }

    // ASCII to UTF16-LE
    _string_descriptor[(i + 1) * 2] = string_descriptors[index][i];
    _string_descriptor[((i + 1) * 2) + 1] = 0x00;
  }

  *ptr = _string_descriptor;
  *length = (uint16_t)_string_descriptor[0];
}
