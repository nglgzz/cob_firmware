#include "usbd.h"

#include <stdbool.h>

#include "clock.h"
#include "core.h"
#include "nrf52840_bitfields.h"
#include "power.h"

usbd_t *const USBD = ((usbd_t *)(USBD_BASE + 0x004));

// Flags used to check that everything is ready for the device to enable the pullup
static bool vbus_detected = false;
static bool usb_power_ready = false;
static bool usbd_ready = false;

#define RINGBUF_LEN 10
// Ring buffer used for setup
// Full speed devices must have a packet size of 64 bytes
static uint64_t ringbuf[RINGBUF_LEN];
static uint32_t read_p = 0;
static uint32_t write_p = 0;

void pullup_maybe();
void setup_ep0();

void init_usbd() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  POWER_INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk |
                   POWER_INTENSET_USBPWRRDY_Msk;

  USBD->INTENSET = USBD_INTENSET_USBEVENT_Msk | USBD_INTENSET_USBRESET_Msk |
                   USBD_INTENSET_EP0SETUP_Msk | USBD_INTENSET_ENDEPIN0_Msk |
                   USBD_INTENSET_EP0DATADONE_Msk;

  NVIC_EnableIRQ(USBD_IRQn);
  NVIC_EnableIRQ(POWER_CLOCK_IRQn);
}

void pullup_maybe() {
  if (USBD->USBPULLUP) {
    return;
  }

  if (!(vbus_detected && usb_power_ready && usbd_ready)) {
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
  USBD->EPIN[0].PTR = (uint32_t)ringbuf[write_p];
  USBD->EPIN[0].MAXCNT = 64;
  USBD->TASKS_STARTEPIN[0] = 1;
}

void POWER_CLOCK_IRQHandler() {
  if (POWER_EVENTS_USBDETECTED) {
    POWER_EVENTS_USBDETECTED = 0;
    vbus_detected = true;

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

    usbd_ready = 1;
    pullup_maybe();
  }
  if (POWER_EVENTS_USBPWRRDY) {
    POWER_EVENTS_USBPWRRDY = 0;
    usb_power_ready = true;
    pullup_maybe();
  }
}

void USBD_IRQHandler() {
  if (USBD->EVENTS_USBEVENT) {
      USBD->EVENTS_USBEVENT = 0;
    }

  if (USBD->EVENTS_USBRESET) {
    USBD->EVENTS_USBRESET = 0;
    setup_ep0();
  }
}
