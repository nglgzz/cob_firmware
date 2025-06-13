#ifndef USBD_H
#define USBD_H

#include <stdbool.h>
#include <stdint.h>

#define USBD_BASE 0x40027000U

void init_usbd();

typedef struct {
  // 5V supply detected on VBUS.
  bool vbus_detected;

  // Internal voltage regulator's worst case settling time has elapsed, indicating to the
  // software that it can enable the USB pull-up to signal a USB connection to the host.
  //
  // The internal voltage regulator converts the VBUS supply into 3.3V that should be used for
  // the data lines.
  bool usb_power_ready;

  // USBD is ready for normal operation.
  bool usbd_ready;

  bool transfer_in_progress;

  bool endpoint0_configured;
} usbd_state_t;
extern usbd_state_t usbd_state;

// void send_report(uint32_t switches);
// typedef struct __attribute__((packed, aligned(4))) {
//   uint8_t modifiers;
//   uint8_t _reserved;
//   uint8_t keys[6];
// } hid_report_t;
// extern hid_report_t hid_report;

typedef struct {
  volatile uint32_t PTR;     // Data pointer
  volatile uint32_t MAXCNT;  // Maximum number of bytes to transfer
  volatile uint32_t AMOUNT;  // Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED[2];
} usbd_ep_t;

typedef struct {
  volatile uint32_t TASKS_STARTEPIN[8];  // (0x004) Captures the EPIN[0].PTR and EPIN[0].MAXCNT
                                         // registers values, and enables endpoint IN 0 to
                                         // respond to traffic from host
  volatile uint32_t
      TASKS_STARTISOIN;  // (0x024) Captures the ISOIN.PTR and ISOIN.MAXCNT registers values,
                         // and enables sending data on ISO endpoint
  volatile uint32_t
      TASKS_STARTEPOUT[8];  // (0x028) Captures the EPOUT[0].PTR and EPOUT[0].MAXCNT registers
                            // values, and enables endpoint 0 to respond to traffic from host
  volatile uint32_t
      TASKS_STARTISOOUT;  // (0x048) Captures the ISOOUT.PTR and ISOOUT.MAXCNT registers
                          // values, and enables receiving of data on ISO endpoint
  volatile uint32_t TASKS_EP0RCVOUT;  // (0x04C) Allows OUT data stage on control endpoint 0
  volatile uint32_t TASKS_EP0STATUS;  // (0x050) Allows status stage on control endpoint 0
  volatile uint32_t
      TASKS_EP0STALL;  // (0x054) Stalls data and status stage on control endpoint 0
  volatile uint32_t TASKS_DPDMDRIVE;  // (0x058) Forces D+ and D- lines into the state defined
                                      // in the DPDMVALUE register
  volatile uint32_t TASKS_DPDMNODRIVE;  // (0x05C) Stops forcing D+ and D- lines into any state
                                        // (USB engine takes control)
  volatile uint32_t __RESERVED[40];
  volatile uint32_t EVENTS_USBRESET;  // (0x100) Signals that a USB reset condition has been
                                      // detected on USB lines
  volatile uint32_t
      EVENTS_STARTED;  // (0x104) Confirms that the EPIN[n].PTR and EPIN[n].MAXCNT, or
                       // EPOUT[n].PTR and EPOUT[n].MAXCNT registers have been captured on all
                       // endpoints reported in the EPSTATUS register
  volatile uint32_t EVENTS_ENDEPIN[8];  // (0x108) The whole EPIN[0] buffer has been consumed.
                                        // The buffer can be accessed safely by software.

  volatile uint32_t EVENTS_EP0DATADONE;  // (0x128) An acknowledged data transfer has taken
                                         // place on the control endpoint

  volatile uint32_t EVENTS_ENDISOIN;  // (0x12C) The whole ISOIN buffer has been consumed. The
                                      // buffer can be accessed safely by software.
  volatile uint32_t
      EVENTS_ENDEPOUT[8];  // (0x130) The whole EPOUT[0] buffer has been consumed. The buffer
                           // can be accessed safely by software.
  volatile uint32_t EVENTS_ENDISOOUT;  // (0x150) The whole ISOOUT buffer has been consumed.
                                       // The buffer can be accessed safely by software.
  volatile uint32_t EVENTS_SOF;  // (0x154) Signals that a SOF (start of frame) condition has
                                 // been detected on USB lines
  volatile uint32_t
      EVENTS_USBEVENT;  // (0x158) An event or an error not covered by specific events has
                        // occurred. Check EVENTCAUSE register to find the cause
  volatile uint32_t EVENTS_EP0SETUP;  // (0x15C) A valid SETUP token has been received (and
                                      // acknowledged) on the control endpoint
  volatile uint32_t EVENTS_EPDATA;  // (0x160) A data transfer has occurred on a data endpoint,
                                    // indicated by the EPDATASTATUS register
  volatile uint32_t __RESERVED2[39];

  volatile uint32_t SHORTS;  // (0x200) Shortcuts between local events and tasks
  volatile uint32_t __RESERVED3[63];
  volatile uint32_t INTEN;     // (0x300) Enable or disable interrupt
  volatile uint32_t INTENSET;  // (0x304) Enable interrupt
  volatile uint32_t INTENCLR;  // (0x308) Disable interrupt
  volatile uint32_t __RESERVED4[61];

  volatile uint32_t EVENTCAUSE;  // (0x400) Details on what caused the USBEVENT event
  volatile uint32_t __RESERVED5[7];
  volatile uint32_t HALTED_EPIN[8];  // (0x420) IN endpoint halted status. Can be used as is as
                                     // response to a GetStatus() request to endpoint.
  volatile uint32_t __RESERVED6[10];
  volatile uint32_t EPSTATUS;      // (0x468) Provides information on which endpoint's EasyDMA
                                   // registers have been captured
  volatile uint32_t EPDATASTATUS;  // (0x46C) Provides information on which endpoint(s) an
                                   // acknowledged data transfer has occurred (EPDATA event)
  volatile uint32_t USBADDR;       // (0x470) Device USB address
  volatile uint32_t __RESERVED7[3];
  volatile uint32_t BMREQUESTTYPE;   // (0x480) SETUP data, byte 0, bmRequestType
  volatile uint32_t BREQUEST;        // (0x484) SETUP data, byte 1, bRequest
  volatile uint32_t WVALUEL;         // (0x488) SETUP data, byte 2, LSB of wValue
  volatile uint32_t WVALUEH;         // (0x48C) SETUP data, byte 3, MSB of wValue
  volatile uint32_t WINDEXL;         // (0x490) SETUP data, byte 4, LSB of wIndex
  volatile uint32_t WINDEXH;         // (0x494) SETUP data, byte 5, MSB of wIndex
  volatile uint32_t WLENGTHL;        // (0x498) SETUP data, byte 6, LSB of wLength
  volatile uint32_t WLENGTHH;        // (0x49C) SETUP data, byte 7, MSB of wLength
  volatile uint32_t SIZE__EPOUT[8];  // (0x4A0) Number of bytes received last in the data stage
                                     // of this OUT endpoint
  volatile uint32_t
      SIZE__ISOOUT;  // (0x4C0) Number of bytes received last on this ISO OUT data endpoint
  volatile uint32_t __RESERVED8[15];
  volatile uint32_t ENABLE;     // (0x500) Enable USB
  volatile uint32_t USBPULLUP;  // (0x504) Control of the USB pull-up
  volatile uint32_t DPDMVALUE;  // (0x508) State D+ and D- lines will be forced into by the
                                // DPDMDRIVE task. The DPDMNODRIVE task reverts the control of
                                // the lines to MAC IP (no forcing)
  volatile uint32_t DTOGGLE;    // (0x50C) Data toggle control and status
  volatile uint32_t EPINEN;     // (0x510) Endpoint IN enable

  volatile uint32_t EPOUTEN;   // (0x514) Endpoint OUT enable
  volatile uint32_t EPSTALL;   // (0x518) STALL endpoints
  volatile uint32_t ISOSPLIT;  // (0x51C) Controls the split of ISO buffers
  volatile uint32_t
      FRAMECNTR;  // (0x520) Returns the current value of the start of frame counter
  volatile uint32_t __RESERVED9[2];
  volatile uint32_t
      LOWPOWER;  // (0x52C) Controls USBD peripheral low power mode during USB suspend
  volatile uint32_t ISOINCONFIG;  // (0x530) Controls the response of the ISO IN endpoint to an
                                  // IN token when no data is ready to be sent
  volatile uint32_t __RESERVED10[51];
  volatile usbd_ep_t EPIN[8];  // (0x600) IN endpoints
  volatile usbd_ep_t ISOIN;    // (0x6A0) ISO endpoint IN
  volatile uint32_t __RESERVED11[19];
  volatile usbd_ep_t EPOUT[8];  // (0x700) OUT endpoints
  volatile usbd_ep_t ISOOUT;    // (0x7A0) ISO endpoint OUT
} usbd_t;

extern usbd_t* const USBD;

#endif  // USBD_H
