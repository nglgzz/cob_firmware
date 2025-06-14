# USBD driver usage

## Usage

### Configuration

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

### Startup

Call `usbd_init()` to start up the peripheral and start listening to USB events.

### Send data

To start an IN transfer (from device to host), use the following function:

```c
void usbd_epin_start(uint8_t ep, uint32_t ptr, uint32_t len);
```

## Useful Resources

- [USB in a Nutshell](https://beyondlogic.org/usbnutshell/usb5.shtml#DeviceDescriptors)
- [Wireshark USB capture setup](https://wiki.wireshark.org/CaptureSetup/USB)
- [usbmon](https://docs.kernel.org/usb/usbmon.html)
- [Introduction to HID report descriptors](https://docs.kernel.org/hid/hidintro.html)

## Debugging Commands

Show USB events in dmesg, these are a very high level overview of what's going on.

```sh
sudo dmesg --notime -wd | grep --color=always usb
```

Log requests in the specified bus (requires disabling secure boot to be able to
circumvent kernel_lockdown). [See here](https://github.com/nglgzz/dots/blob/ubuntu/dots/.config/zsh/aliases/shell.sh#L133-L201) for a helper to parse the output.

```sh
sudo cat "/sys/kernel/debug/usb/usbmon/${bus_number}u"
```

Show the HID report descriptor of a connected device (hid-decode is part of
hid-tools which can be installed via pip).

```sh
hexdump /sys/bus/hid/devices/${bus_number}:${vid}:${pid}:*/report_descriptor
# or
hid-decode /sys/bus/hid/devices/${bus_number}:${vid}:${pid}:*/report_descriptor
```

Show error codes and their meaning.

```sh
cat /usr/include/asm-generic/errno*.h
```
