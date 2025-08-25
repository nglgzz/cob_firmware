# USB Device (USBD)
- [USB Overview](#usb-overview)
  - [Key USB Concepts](#key-usb-concepts)
- [USBD API Reference](#usbd-api-reference)
  - [State Management](#state-management)
  - [Initialization](#initialization)
  - [Descriptor Configuration](#descriptor-configuration)
  - [Data Transfer](#data-transfer)
- [Implementation Notes](#implementation-notes)
  - [Hardware Anomalies](#hardware-anomalies)
  - [Connection Sequence](#connection-sequence)
  - [Control Transfer Handling](#control-transfer-handling)
- [Usage Example](#usage-example)
- [Troubleshooting Commands](#troubleshooting-commands)
- [Useful Resources](#useful-resources)

## USB Overview

Universal Serial Bus (USB) is a standard for connecting peripherals to a host
computer. The process of configuring a USB device when it's first connected is
called enumeration. During enumeration, the host identifies the device, assigns
it an address, and determines its configuration.

### Key USB Concepts

1. Endpoints: Communication channels between the host and device

   - Control Endpoints: Used for device configuration (every device has EP0)
   - Bulk Endpoints: Used for reliable data transfer
   - Interrupt Endpoints: Used for small, time-sensitive data
   - Isochronous Endpoints: Used for streaming data with timing guarantees

2. Descriptors: Data structures that describe the device

   - Device Descriptor: Contains basic device information
   - Configuration Descriptor: Describes power requirements and interfaces
   - Interface Descriptor: Describes a functional group within a configuration
   - Endpoint Descriptor: Describes endpoint characteristics

3. Control Transfers: Configuration messages with three stages

   - Setup Stage: Host sends request details
   - Data Stage: Data exchange (optional)
   - Status Stage: Acknowledges successful completion

For more detailed information on USB concepts:

- **USB in a Nutshell**: [Introduction](https://www.beyondlogic.org/usbnutshell/usb1.shtml),
  [Protocols](https://www.beyondlogic.org/usbnutshell/usb3.shtml),
  [Endpoint types](https://www.beyondlogic.org/usbnutshell/usb4.shtml),
  [USB Descriptors](https://www.beyondlogic.org/usbnutshell/usb5.shtml)
- [USB 2.0 Specification](https://www.usb.org/document-library/usb-20-specification)

## USBD API Reference

The USBD API provides low-level access to the USB peripheral on the nRF52840
microcontroller. It handles USB device initialization, enumeration, and data
transfers.

### State Management

```c
typedef struct {
  bool vbus_detected;        // 5V supply detected on VBUS  bool usb_power_ready;
  bool usb_power_ready;      // Internal voltage regulator ready
  bool usbd_ready;           // USBD peripheral ready for operation
  bool transfer_in_progress; // Transfer lock (only one EasyDMA at a time)
  uint16_t configuration;    // Current selected configuration
} usbd_state_t;

extern usbd_state_t usbd_state;
```

### Initialization

```c
void init_usbd();
```

Initializes the USB device peripheral:

- Starts the high-frequency clock
- Enables USB detection, removal, and power-ready interrupts
- Enables USB event, reset, and setup interrupts
- Configures the interrupt controller

> [!NOTE]
> This function does not enable the USB peripheral or connect the device.
> The peripheral is enabled when USB power is detected, and the device is connected when all conditions are met.

### Descriptor Configuration

**Required descriptors**

For a USB device to properly enumerate, you must define a complete descriptor hierarchy:

- Device Descriptor: Contains basic device information (vendor ID, product ID, etc.)
- Configuration Descriptor: Contains power requirements and interface count
- Interface Descriptor: Defines a functional group (like a keyboard or mouse)
- Endpoint Descriptors: Define communication channels beyond the default control endpoint

> [!IMPORTANT]
>
> When setting up USB descriptors, pay careful attention to:
>
> - HID Report Descriptor Length: Must be correctly specified in the HID descriptor
> - Endpoint Max Packet Size: Must match the size declared in endpoint descriptors
>
> Incorrect length values will cause enumeration failures or unpredictable device behavior.

**Descriptor handlers**

```c
void USBD_GetDescriptor_Device(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_Configuration(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_String(uint8_t** ptr, uint16_t* length, uint8_t index);
void USBD_GetDescriptor_HIDReport(uint8_t** ptr, uint16_t* length, uint8_t index);
```

These handlers are called by the USB stack when the host requests the corresponding
descriptor. You must implement each one to:

- Set `*ptr` to point to your descriptor data
- Set `*length` to the descriptor's length in bytes
- Optionally use `index` to differentiate between multiple descriptors of the same type

The following utils are available to symplify handling requests for configuration
and string descriptors.

**Helper utilities**

This utility concatenates multiple descriptors (e.g., configuration, interface,
and endpoint) into a single buffer.

```c
uint16_t USBD_DESC_CombineDescriptors(
    uint8_t combined_descriptor[],        // Output buffer for combined descriptor
    descriptor_ptr_t descriptor_list[],   // Array of descriptor pointers to combine
    size_t length                        // Number of descriptors in the list
);
```

This utility handles the conversion of C strings to USB string descriptors,
including proper UNICODE formatting.

```c
void USBD_DESC_GetString(
    char* string_descriptors[],          // Array of C strings to convert to USB string descriptors
    uint8_t** ptr,                       // Output pointer (set by the function)
    uint16_t* length,                    // Output length (set by the function)
    uint8_t index                        // String index requested by host
);
```

**Reset handler**

```c
void USBD_Reset_Handler();
```

Implement this function to:

- Enable and configure endpoints beyond the default control endpoint (EP0)
- Reinitialize any device state after a USB reset

### Data Transfer

```c
void usbd_epin_start(uint8_t ep, uint32_t ptr, uint32_t len);
```

Initiates a transfer on an IN endpoint (device to host):

- ep: Endpoint number (0-7)
- ptr: Pointer to the data to be sent
- len: Length of data in bytes

> [!Important]
> Only one transfer can occur at a time due to EasyDMA limitations. The function
> implements safeguards to prevent concurrent transfers.

## Implementation Notes

### Hardware Anomalies

The driver includes workarounds for nRF52840 hardware anomalies:

1. Anomaly 187: USB cannot be enabled
   - Applies special register sequences when enabling the USB peripheral
1. Anomaly 199: USBD cannot receive tasks during DMA
   - Applies special register sequences around DMA transfers

### Connection Sequence

The driver implements the following connection sequence:

- Wait for VBUS detection (USB cable connected)
- Enable the USB peripheral with workarounds
- Wait for USB power regulator ready signal
- Wait for USBD peripheral ready signal
- Enable the USB pullup to connect to the host

### Control Transfer Handling

The driver automatically handles standard USB requests:

- `GET_DESCRIPTOR`: Calls appropriate descriptor callback based on type
- `SET_CONFIGURATION`: Updates the configuration state
- `SET_ADDRESS`: Handled by hardware, transparent to software

## Usage Example

```c
// Define USB descriptors
static uint8_t device_descriptor[] = { /* ... */ };
static uint8_t config_descriptor[] = { /* ... */ };

// Implement descriptor callbacks
void USBD_GetDescriptor_Device(uint8_t **ptr, uint16_t *length, uint8_t index) {
    *ptr = device_descriptor;
    *length = sizeof(device_descriptor);
}

void USBD_GetDescriptor_Configuration(uint8_t **ptr, uint16_t *length, uint8_t index) {
    *ptr = config_descriptor;
    *length = sizeof(config_descriptor);
}

void USBD_Reset_Handler() {
    // Enable IN endpoint 1 for HID reports
    USBD->EPINEN |= (1 << 1);
}

void send_report(uint8_t *report_data, size_t report_len) {
    // Wait until bus is configured
    if (usbd_state.configuration == 0) return;

    // Send data on endpoint 1
    usbd_epin_start(1, (uint32_t)report_data, report_len);
}

int main(void) {
    init_usbd();

    while(1) {
        // Application logic
    }
}
```

## Troubleshooting Commands

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

## Useful Resources

- [USB in a Nutshell](https://beyondlogic.org/usbnutshell/usb5.shtml#DeviceDescriptors)
- [Wireshark USB capture setup](https://wiki.wireshark.org/CaptureSetup/USB)
- [usbmon](https://docs.kernel.org/usb/usbmon.html)
- [Introduction to HID report descriptors](https://docs.kernel.org/hid/hidintro.html)
