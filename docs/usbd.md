# USBD driver usage

## Configuration

Define the following descriptors and their respective handlers:

- Device
- Configuration
- Interface
- Endpoint

```c
void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_String(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_HIDReport(uint8_t** ptr, uint16_t* length, uint8_t index);
```

The following utils are available to symplify handling requests for configuration and string descriptors.

```c
uint16_t USBD_DESC_CombineDescriptors(uint8_t combined_descriptor[],
                                             descriptor_ptr_t descriptor_list[],
                                             size_t length);

void USBD_DESC_GetString(char* string_descriptors[], uint8_t** ptr, uint16_t* length,
                                uint8_t index);
```

Define the RESET handler in which you should enable the relevant endpoints.

```c
void USBD_Reset_Handler();
```

## Startup

Call `usbd_init()` to start up the peripheral and start listening to USB events.

## Send data

To start an IN transfer (from device to host), use the following function:

```c
void usbd_epin_start(uint8_t ep, uint32_t ptr, uint32_t len);
```

## Useful Resources

- [USB in a Nutshell](https://beyondlogic.org/usbnutshell/usb5.shtml#DeviceDescriptors)
- [Wireshark USB capture setup](https://wiki.wireshark.org/CaptureSetup/USB)
- [usbmon](https://docs.kernel.org/usb/usbmon.html)
- [Introduction to HID report descriptors](https://docs.kernel.org/hid/hidintro.html)
