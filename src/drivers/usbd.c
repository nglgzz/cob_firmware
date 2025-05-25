#include "usbd.h"

#include <stdbool.h>

#include "clock.h"
#include "core.h"
#include "power.h"

usbd_t* const USBD = ((usbd_t*)(USBD_BASE + 0x004));

static bool vbus_detected = false;
static bool usb_power_ready = false;
void pullup_maybe();

void init_usbd() {
  CLOCK->TASKS_HFCLKSTART = 1;
  while (CLOCK->EVENTS_HFCLKSTARTED == 0);

  POWER_INTENSET = POWER_INTENSET_USBDETECTED_Msk | POWER_INTENSET_USBREMOVED_Msk |
                   POWER_INTENSET_USBPWRRDY_Msk;
  NVIC_SETENA |= (1 << USBD_IRQn) | (1 << POWER_CLOCK_IRQn);

  USBD->INTENSET = USBD_INTENSET_USBEVENT_Msk;
}

void pullup_maybe() {
  if (USBD->USBPULLUP) {
    return;
  }

  if (!(vbus_detected && usb_power_ready)) {
    return;
  }

  // To make sure that all resources in USBD are available and the dedicated USB voltage
  // regulator stabilized, the following is recommended:
  //   - Enable USBD only after VBUS has been detected
  //   - Turn the USB pull-up on after the following events have occurred:
  //      - USBPWRRDY
  //      - USBEVENT, with the READY condition flagged in EVENTCAUSE
  // I didn't manage to receive any USBEVENT yet, so for now I'm ignoring that bit, and it
  // seems to do the trick.
  USBD->USBPULLUP = 1;
}

void POWER_CLOCK_IRQHandler() {
  if (POWER_EVENTS_USBDETECTED) {
    POWER_EVENTS_USBDETECTED = 0;
    vbus_detected = true;
    USBD->ENABLE = 1;
    pullup_maybe();
  }
  if (POWER_EVENTS_USBREMOVED) {
    POWER_EVENTS_USBREMOVED = 0;
    vbus_detected = false;
  }
  if (POWER_EVENTS_USBPWRRDY) {
    POWER_EVENTS_USBPWRRDY = 0;
    usb_power_ready = true;
    pullup_maybe();
  }
}

void USBD_IRQHandler() {
  if (USBD->EVENTS_USBEVENT) {
    if (USBD->EVENTCAUSE & USBD_EVENTCAUSE_READY_Msk) {
      // TODO: This code isn't reached currently, need to continue with the implementation to
      // see if it works
      USBD->EVENTS_USBEVENT = 0;
      USBD->EVENTCAUSE &= (~USBD_EVENTCAUSE_READY_Msk);
      pullup_maybe();
    }
  }
}
