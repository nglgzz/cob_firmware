
#include "usbd_descriptors.h"

#include "utils.h"

// ----------------------
//        STATUS
// ----------------------
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

// ----------------------
//     CONFIGURATION
// ----------------------
inline uint16_t usbd_descriptors_set_configuration_descriptor(
    uint8_t combined_descriptor[], descriptor_ptr_t descriptor_list[], size_t length) {
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

// ----------------------
//        STRINGS
// ----------------------

static uint8_t _string_descriptor[128];
void USBD_DESCRIPTORS_String(char *string_descriptors[], uint8_t **ptr, uint16_t *length,
                             uint8_t index) {
  if (index == 0) {
    // LANGID descriptor
    _string_descriptor[0] = 0x04;                         // bLength
    _string_descriptor[1] = USBD_DESCRIPTOR_TYPE_String;  // bDescriptorType
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
      _string_descriptor[0] = (i + 1) * 2;                  // bLength
      _string_descriptor[1] = USBD_DESCRIPTOR_TYPE_String;  // bDescriptorType
      break;
    }

    // ASCII to UTF16-LE
    _string_descriptor[(i + 1) * 2] = string_descriptors[index][i];
    _string_descriptor[((i + 1) * 2) + 1] = 0x00;
  }

  *ptr = _string_descriptor;
  *length = (uint16_t)_string_descriptor[0];
}

void USBD_DefaultHandler(uint8_t **ptr, uint16_t *length, uint8_t index) {}

void USBD_GetDescriptor_Device(uint8_t **ptr, uint16_t *length, uint8_t index)
    __attribute__((weak, alias("USBD_DefaultHandler")));
void USBD_GetDescriptor_Configuration(uint8_t **ptr, uint16_t *length, uint8_t index)
    __attribute__((weak, alias("USBD_DefaultHandler")));
void USBD_GetDescriptor_String(uint8_t **ptr, uint16_t *length, uint8_t index)
    __attribute__((weak, alias("USBD_DefaultHandler")));
void USBD_GetDescriptor_HIDReport(uint8_t **ptr, uint16_t *length, uint8_t index)
    __attribute__((weak, alias("USBD_DefaultHandler")));
