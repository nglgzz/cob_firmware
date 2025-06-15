#include "usbd.h"

#include <stdbool.h>
#include <stddef.h>

#include "clock.h"
#include "core.h"
#include "nrf52840_bitfields.h"
#include "power.h"
#include "usbd_desc.h"
#include "utils.h"

usbd_t *const USBD = ((usbd_t *)(USBD_BASE + 0x004U));
usbd_state_t usbd_state = {};

static inline void usbd_pullup();
static inline void usbd_get_descriptor_handler();
static inline void usbd_epin0_transfer(uint32_t ptr, uint16_t len, uint16_t max_len);

void USBD_noop() {}
void USBD_Reset_Handler() __attribute__((weak, alias("USBD_noop")));

void init_usbd() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  POWER_INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk |
                   POWER_INTENSET_USBPWRRDY_Msk;

  USBD->INTENSET =
      USBD_INTENSET_USBEVENT_Msk | USBD_INTENSET_USBRESET_Msk | USBD_INTENSET_EP0SETUP_Msk;

  NVIC_EnableIRQ(USBD_IRQn);
  NVIC_EnableIRQ(POWER_CLOCK_IRQn);
}

void POWER_CLOCK_IRQHandler() {
  if (POWER_EVENTS_USBDETECTED) {
    POWER_EVENTS_USBDETECTED = 0;
    usbd_state.vbus_detected = true;

    // The following lines are magic incantations from nRF52840 Engineering D Errata v1.5, to
    // address the following issue:
    //
    // 3.22 [187] USBD: USB cannot be enabled
    //   This anomaly applies to IC Rev.Engineering D, build codes CKAA - DA0, QIAA - DA0. It
    //   was inherited from the previous IC revision Revision 1.
    //   https://docs.nordicsemi.com/bundle/errata_nRF52840_EngD/page/ERR/nRF52840/EngineeringD/latest/err_840.html
    //   https://docs.nordicsemi.com/bundle/errata_nRF52840_EngD/page/ERR/nRF52840/EngineeringD/latest/anomaly_840_187.html
    //
    // Symptoms
    //   After writing to NRF_USBD->ENABLE, no EVENTS_USBEVENT is triggered, and
    //   USB->EVENTCAUSE is not updated.
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
    *(volatile uint32_t *)0x4006ED14 = 0x00000003;
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;

    USBD->ENABLE = 1;
    // Waiting for peripheral to enable, this should take a few µs
    while (0 == (USBD->EVENTCAUSE & USBD_EVENTCAUSE_READY_Msk));
    USBD->EVENTCAUSE &= ~USBD_EVENTCAUSE_READY_Msk;

    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
    *(volatile uint32_t *)0x4006ED14 = 0x00000000;
    *(volatile uint32_t *)0x4006EC00 = 0x00009375;
    // End of magic incantations

    usbd_state.usbd_ready = true;
    usbd_pullup();
  }
  if (POWER_EVENTS_USBPWRRDY) {
    POWER_EVENTS_USBPWRRDY = 0;
    usbd_state.usb_power_ready = true;
    usbd_pullup();
  }
  if (POWER_EVENTS_USBREMOVED) {
    POWER_EVENTS_USBREMOVED = 0;

    // Wait for potential transfers to end and then disable USBD.
    while (usbd_state.transfer_in_progress);

    usbd_state.vbus_detected = false;
    usbd_state.usb_power_ready = false;
    usbd_state.usbd_ready = false;

    // After writing Disabled to this register, reading the register will return Enabled until
    // USBD is completely disabled.
    USBD->ENABLE = 0;
    while (USBD->ENABLE == 1);

    USBD->USBPULLUP = 0;
  }
}

void USBD_IRQHandler() {
  if (USBD->EVENTS_USBEVENT) {
    USBD->EVENTS_USBEVENT = 0;

    // TODO: check USBD->EVENTCAUSE and handle SUSPEND, RESUME, and USBWUALLOWED.
  }

  if (USBD->EVENTS_USBRESET) {
    USBD->EVENTS_USBRESET = 0;
    usbd_state.transfer_in_progress = false;
    usbd_state.configuration = 0;

    USBD_Reset_Handler();
  }

  // A control transfer is a transfer used to configure the USB device. It contains 3 stages:
  // - setup
  // - data (optional)
  // - status
  //
  // Each control transfer can be one of the following types:
  // - control read
  // - control read no data
  // - control write
  // - control write no data
  //
  // EVENTTS_EP0SETUP indicates that the data from the setup stage is available in the USBD
  // registers.
  //
  // After receiving the SETUP token, the USB controller will not accept (NAK) any incoming IN
  // or OUT tokens until the software has finished decoding the command, determined the type of
  // transfer, and prepared for the next stage (data or status) appropriately.
  if (USBD->EVENTS_EP0SETUP) {
    USBD->EVENTS_EP0SETUP = 0;
    USBD->TASKS_EP0STALL = 0;

    // Read the setup packet
    uint8_t bmRequestType = USBD->BMREQUESTTYPE;
    uint8_t bRequest = USBD->BREQUEST;
    uint16_t wIndex = ((uint16_t)USBD->WINDEXH << 8) | ((uint16_t)USBD->WINDEXL);
    uint16_t wLength = ((uint16_t)USBD->WLENGTHH << 8) | ((uint16_t)USBD->WLENGTHL);

    uint8_t requestDirection = bmRequestType & USBD_BMREQUESTTYPE_DIRECTION_Msk;
    uint8_t requestType = bmRequestType & USBD_BMREQUESTTYPE_TYPE_Msk;

    if (requestType == USBD_BMREQUESTTYPE_TYPE_Standard) {
      switch (bRequest) {
        case USBD_BREQUEST_BREQUEST_STD_GET_DESCRIPTOR:
          usbd_get_descriptor_handler();
          break;

        case USBD_BREQUEST_BREQUEST_STD_SET_CONFIGURATION:
          usbd_state.configuration = USBD->WVALUEL;
          USBD->TASKS_EP0STATUS = 1;
          break;

        case USBD_BREQUEST_BREQUEST_STD_SET_DESCRIPTOR:
          // Optional, will not implement.
          USBD->TASKS_EP0STATUS = 1;
          break;

        case USBD_BREQUEST_BREQUEST_STD_GET_STATUS:
        case USBD_BREQUEST_BREQUEST_STD_SET_ADDRESS:
          // Handled by the hardware, software should ignore this.
          break;

        default:
          //  Stall unsupported requests.
          USBD->TASKS_EP0STALL = 1;
      }
    } else if (requestType == USBD_BMREQUESTTYPE_TYPE_Class) {
      // TODO: this assumes HID class currently
      switch (bRequest) {
          // SET_REPORT
          // The Set_Report request allows the host to send a report to the device, possibly
          // setting the state of input, output, or feature controls.
        case 0x09:
          // TODO: needs implementation
          USBD->TASKS_EP0STATUS = 1;
          break;

        default:
          //  Stall unsupported requests
          USBD->TASKS_EP0STALL = 1;
      }
    }
  }

  if (USBD->EVENTS_ENDEPIN[1]) {
    USBD->EVENTS_ENDEPIN[1] = 0;
    usbd_state.transfer_in_progress = false;
  }
}

static inline void usbd_pullup() {
  if (USBD->USBPULLUP) {
    return;
  }

  if (!(usbd_state.vbus_detected && usbd_state.usb_power_ready && usbd_state.usbd_ready)) {
    return;
  }

  // To make sure that all resources in USBD are available and the dedicated USB voltage
  // regulator stabilized, the following is recommended:
  //   - Enable USBD only after VBUS has been detected
  //   - Turn the USB pull-up on after the following events have occurred:
  //      - USBPWRRDY
  //      - USBEVENT, with the READY condition flagged in EVENTCAUSE
  if (USBD->USBPULLUP == 0) {
    USBD->USBPULLUP = 1;
  }
}

static inline void usbd_get_descriptor_handler() {
  // GET_DESCRIPTOR
  // - wValueH: descriptor type
  // - wValueL: descriptor index (used when there is more than one descriptor available
  //            for a particular type)
  // - wIndex: 0 or language ID (in case the hardware is localized?)
  // - wLength: max size of descriptor that can be sent by the device
  uint8_t descriptor_type = USBD->WVALUEH;
  uint8_t descriptor_index = USBD->WVALUEL;

  uint16_t wIndex = (((uint16_t)USBD->WINDEXH) << 8) | ((uint16_t)USBD->WINDEXL);
  uint16_t wLength =
      (((uint16_t)USBD->WLENGTHH & 0xFF) << 8) | ((uint16_t)USBD->WLENGTHL & 0xFF);

  uint8_t *descriptor = NULL;
  uint16_t descriptor_length = 0;

  switch (descriptor_type) {
    case USBD_DESC_TYPE_Device:
      USBD_GetDescriptor_Device(&descriptor, &descriptor_length, descriptor_index);
      break;
    case USBD_DESC_TYPE_Configuration:
      USBD_GetDescriptor_Configuration(&descriptor, &descriptor_length, descriptor_index);
      break;
    case USBD_DESC_TYPE_String:
      USBD_GetDescriptor_String(&descriptor, &descriptor_length, descriptor_index);
      break;
    case USBD_DESC_TYPE_DeviceQualifier:
      // Only relevant for high speed
      break;
    case USBD_DESC_TYPE_HIDReport:
      USBD_GetDescriptor_HIDReport(&descriptor, &descriptor_length, descriptor_index);
      break;
  }

  if (descriptor) {
    // Send the descriptor
    usbd_epin0_transfer((uint32_t)descriptor, descriptor_length, wLength);
  } else {
    USBD->TASKS_EP0STALL = 1;
  }
}

static inline void usbd_epin0_transfer(uint32_t ptr, uint16_t len, uint16_t max_len) {
  usbd_state.transfer_in_progress = true;
  // If what we're trying to send is longer than the max total length determined by the
  // host, then we'll need to truncate the data we send.
  uint16_t send_len = (len < max_len) ? len : max_len;

  // If what we're trying to send is longer than the max packet size defined in the device
  // descriptor, then we'll need to send the data in multiple packets.
  uint16_t max_packet_size = 64;

  uint16_t bytes_sent = 0;
  while (bytes_sent < send_len) {
    uint16_t packet_size = ((send_len - bytes_sent) < max_packet_size)
                               ? (send_len - bytes_sent)
                               : max_packet_size;

    usbd_epin_start(0, ptr + bytes_sent, packet_size);
    bytes_sent += packet_size;
  }

  USBD->TASKS_EP0STATUS = 1;
  usbd_state.transfer_in_progress = false;
}

inline void usbd_epin_start(uint8_t ep, uint32_t ptr, uint32_t len) {
  // TODO: re-enable this
  // if (ep != 0 && usbd_state.transfer_in_progress) {
  //   // Drop requests from other endpoints if there are transfers in progress in endpoint 0.
  //   // This is to avoid blocking setup requests and potentially ending with a deadlock
  //   // (i.e. endpoint 0 waiting for another endpoint to complete its transfer and the other
  //   // endpoint not being able to complete the transfer if the setup is not complete).
  //   return;
  // }

  if (ep != 0) {
    // Wait until the current transfer is done before starting another one.
    while (usbd_state.transfer_in_progress);
    usbd_state.transfer_in_progress = true;
  }

  if (ep == 0) {
    // Clear the event just in case, since we're gonna wait for it.
    USBD->EVENTS_EP0DATADONE = 0;
  }

  uint8_t ep_n = ep % 8;

  // The following lines are magic incantations from nRF52840 Engineering D Errata v1.5, to
  // address the following issue:
  //
  // 3.30 [199] USBD: USBD cannot receive tasks during DMA
  //   This anomaly applies to IC Rev. Engineering D, build codes CKAA-DA0, QIAA-DA0.
  //   It was inherited from the previous IC revision Revision 1.
  //   https://docs.nordicsemi.com/bundle/errata_nRF52840_EngD/page/ERR/nRF52840/EngineeringD/latest/err_840.html
  //   https://docs.nordicsemi.com/bundle/errata_nRF52840_EngD/page/ERR/nRF52840/EngineeringD/latest/anomaly_840_199.html
  //
  // Symptoms
  //   The USBD does not perform incoming tasks.
  //
  // To enable incoming tasks when using DMA, use the following code when starting a DMA
  // transfer:
  *(volatile uint32_t *)0x40027C1C = 0x00000082;
  USBD->EPIN[ep_n].PTR = ptr;
  USBD->EPIN[ep_n].MAXCNT = len;
  USBD->TASKS_STARTEPIN[ep_n] = 1;

  if (ep_n == 0) {
    while (USBD->EVENTS_EP0DATADONE == 0);
    USBD->EVENTS_EP0DATADONE = 0;
  } else {
    while (USBD->EVENTS_ENDEPIN[ep_n] == 0);
    USBD->EVENTS_ENDEPIN[ep_n] = 0;
    usbd_state.transfer_in_progress = false;
  }

  // After the DMA transfer is completed, use:
  *(volatile uint32_t *)0x40027C1C = 0x00000000;
}
