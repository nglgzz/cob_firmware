#include "usbd.h"

#include <stdbool.h>
#include <stddef.h>

#include "clock.h"
#include "core.h"
#include "nrf52840_bitfields.h"
#include "power.h"
#include "usbd_descriptors.h"
#include "usbd_hid.h"
#include "utils.h"

usbd_t *const USBD = ((usbd_t *)(USBD_BASE + 0x004U));
usbd_state_t usbd_state = {};

// typedef struct {
//   // 5V supply detected on VBUS.
//   bool vbus_detected;

//   // Internal voltage regulator's worst case settling time has elapsed, indicating to the
//   // software that it can enable the USB pull-up to signal a USB connection to the host.
//   //
//   // The internal voltage regulator converts the VBUS supply into 3.3V that should be used
//   for
//   // the data lines.
//   bool usb_power_ready;

//   // USBD is ready for normal operation.
//   bool usbd_ready;

//   bool transfer_in_progress;

//   bool endpoint0_configured;
// } usbd_state_t;

void init_usbd() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  POWER_INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk |
                   POWER_INTENSET_USBPWRRDY_Msk;

  USBD->INTENSET = USBD_INTENSET_USBEVENT_Msk | USBD_INTENSET_USBRESET_Msk |
                   USBD_INTENSET_EP0SETUP_Msk | USBD_INTENSET_ENDEPIN0_Msk |
                   USBD_INTENSET_EP0DATADONE_Msk | USBD_INTENSET_ENDEPIN1_Msk;

  // It is possible to enable a shortcut from the EP0DATADONE event to the EP0STATUS task,
  // typically if the data stage is expected to take a single transfer. If there is no data
  // stage, the software can initiate the status stage through the EP0STATUS task right away,
  // as as shown in the following figure.
  USBD->SHORTS = USBD_SHORTS_EP0DATADONE_EP0STATUS_Msk;

  NVIC_EnableIRQ(USBD_IRQn);
  NVIC_EnableIRQ(POWER_CLOCK_IRQn);
}

void pullup_maybe();

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

    usbd_state.usbd_ready = 1;
    pullup_maybe();
  }
  if (POWER_EVENTS_USBPWRRDY) {
    POWER_EVENTS_USBPWRRDY = 0;
    usbd_state.usb_power_ready = true;
    pullup_maybe();
  }
  if (POWER_EVENTS_USBREMOVED) {
    POWER_EVENTS_USBREMOVED = 0;
    usbd_state.vbus_detected = false;
    // TODO: wait for potential transfers to end and then disable USBD
    USBD->ENABLE = 0;
    while (USBD->ENABLE == 1);
    USBD->USBPULLUP = 0;
  }
}

void setup_ep0();
void handle_get_descriptor();

// hid_report_t hid_report = {
//     .modifiers = 0,
//     ._reserved = 0,
//     .keys = {0},
// };
// uint8_t hid_keycodes[] = {0x00, 0x17, 0x15, 0x08};
// uint32_t current_switches = 0;

// void send_report(uint32_t switches) {
//   current_switches = switches;

//   // HID report
//   if (switches & 0x01)
//     // Left shift
//     hid_report.modifiers = 0x02;
//   else
//     hid_report.modifiers = 0x00;

//   for (int i = 1; i < 4; i++) {
//     if (switches & (0x01 << i))
//       hid_report.keys[i + 2] = hid_keycodes[i];
//     else
//       hid_report.keys[i + 2] = 0x00;
//   }

//   usbd_state.transfer_in_progress = true;
//   USBD->EPIN[1].PTR = (uint32_t)&hid_report;
//   USBD->EPIN[1].MAXCNT = 8;
//   USBD->TASKS_STARTEPIN[1] = 1;
// }

#define RINGBUF_LEN 64
// Ring buffer used for setup
// Full speed devices must have a packet size of 64 bytes
static uint8_t read_ringbuf[RINGBUF_LEN];
static uint8_t write_ringbuf[RINGBUF_LEN];
static uint32_t read_p = 0;
static uint32_t write_p = 0;

void USBD_IRQHandler() {
  if (USBD->EVENTS_USBEVENT) {
    USBD->EVENTS_USBEVENT = 0;
  }

  if (USBD->EVENTS_USBRESET) {
    USBD->EVENTS_USBRESET = 0;
    usbd_state.transfer_in_progress = false;
    usbd_state.endpoint0_configured = false;

    USBD->EPINEN = 0x3;
    setup_ep0();
  }

  if (USBD->EVENTS_ENDEPIN[1]) {
    USBD->EVENTS_ENDEPIN[1] = 0;
    usbd_state.transfer_in_progress = false;
  }

  if (!usbd_state.transfer_in_progress && usbd_state.endpoint0_configured) {
    usbd_hid_send(current_switches);
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
          handle_get_descriptor();
          break;

        case USBD_BREQUEST_BREQUEST_STD_SET_CONFIGURATION:
          USBD->TASKS_EP0STATUS = 1;
          break;
        case USBD_BREQUEST_BREQUEST_STD_SET_DESCRIPTOR:
          USBD->TASKS_EP0STATUS = 1;
          break;

        case USBD_BREQUEST_BREQUEST_STD_GET_STATUS:
        case USBD_BREQUEST_BREQUEST_STD_SET_ADDRESS:
          break;

        default:
          //  Stall unsupported requests
          USBD->TASKS_EP0STALL = 1;
      }
    } else if (requestType == USBD_BMREQUESTTYPE_TYPE_Class) {
      switch (bRequest) {
        // SET_REPORT
        case 0x09:
          USBD->TASKS_EP0STATUS = 1;
          break;

        default:
          //  Stall unsupported requests
          USBD->TASKS_EP0STALL = 1;
      }
    }
  }

  // if (USBD->EVENTS_ENDEPIN[0]) {
  //   USBD->EVENTS_ENDEPIN[0] = 0;
  //   USBD->TASKS_EP0STATUS = 1;
  // }

  // if (USBD->EVENTS_EP0DATADONE) {
  //   USBD->EVENTS_EP0DATADONE = 0;

  //   // read_p = (read_p + 1) % RINGBUF_LEN;
  //   // setup_ep0();
  //   // ...
  //   // or
  //   // ...

  //   USBD->TASKS_EP0STATUS = 1;
  // }
}

void pullup_maybe() {
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
    setup_ep0();
  }
}

void setup_ep0() {
  // Point the USB peripheral to our buffer
  USBD->EPIN[0].PTR = (uint32_t)write_ringbuf;
  USBD->EPIN[0].MAXCNT = 64;
  // write_p = (write_p + 1) % RINGBUF_LEN;

  // If no other tranfers are going on.
  //
  // An ENDEPIN[0] event will be generated when the data has been transferred from memory to
  // the USBD peripheral.
  //
  // The STARTED event confirms that the values of the .PTR and .MAXCNT registers of the
  // endpoints flagged in register EPSTATUS have been captured. Those can then be modified by
  // software for the next transfer.
  USBD->TASKS_STARTEPIN[0] = 1;
}

void handle_get_descriptor() {
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
    case USBD_DESCRIPTOR_TYPE_Device:
      USBD_GetDescriptor_Device(&descriptor, &descriptor_length, descriptor_index);
      break;
    case USBD_DESCRIPTOR_TYPE_Configuration:
      USBD_GetDescriptor_Configuration(&descriptor, &descriptor_length, descriptor_index);
      break;
    case USBD_DESCRIPTOR_TYPE_HIDReport:
      descriptor = hid_report_descriptor;
      descriptor_length = sizeof(hid_report_descriptor);
      break;
    case USBD_DESCRIPTOR_TYPE_String:
      USBD_GetDescriptor_String(&descriptor, &descriptor_length, descriptor_index);
      break;
  }

  if (descriptor) {
    // Send the descriptor
    USBD->EPIN[0].PTR = (uint32_t)descriptor;
    USBD->EPIN[0].MAXCNT = (wLength < descriptor_length) ? wLength : descriptor_length;
    USBD->TASKS_STARTEPIN[0] = 1;
  } else {
    USBD->TASKS_EP0STALL = 1;
  }
}
