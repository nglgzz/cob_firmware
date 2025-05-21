#ifndef USBD_H
#define USBD_H

#include <stdint.h>

#define USBD_BASE 0x40027000

typedef struct {
  volatile uint32_t TASKS_STARTEPIN[8];  // (0x004) Captures the EPIN[0].PTR and EPIN[0].MAXCNT
                                         // registers values, and enables endpoint IN 0 to
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

  volatile uint32_t __RESERVED1[7];
  volatile uint32_t EVENTS_EP0DATADONE;  // (0x128) An acknowledged data transfer has taken
                                         // place on the control endpoint

  volatile uint32_t EVENTS_ENDISOIN;  // (0x12C) The whole ISOIN buffer has been consumed. The
                                      // buffer can be accessed safely by software.
  volatile uint32_t
      EVENTS_ENDEPOUT[8];  // (0x130) The whole EPOUT[0] buffer has been consumed. The buffer
                           // can be accessed safely by software.
  volatile uint32_t __RESERVED2[7];
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
  volatile uint32_t __RESERVED3[15];

  volatile uint32_t SHORTS;  // (0x200) Shortcuts between local events and tasks
  volatile uint32_t __RESERVED4[63];
  volatile uint32_t INTEN;     // (0x300) Enable or disable interrupt
  volatile uint32_t INTENSET;  // (0x304) Enable interrupt
  volatile uint32_t INTENCLR;  // (0x308) Disable interrupt
  volatile uint32_t __RESERVED5[61];

  volatile uint32_t EVENTCAUSE;  // (0x400) Details on what caused the USBEVENT event
  volatile uint32_t __RESERVED6[7];
  volatile uint32_t HALTED_EPIN[8];  // (0x420) IN endpoint halted status. Can be used as is as
                                     // response to a GetStatus() request to endpoint.
  volatile uint32_t __RESERVED7[17];
  volatile uint32_t EPSTATUS;      // (0x468) Provides information on which endpoint's EasyDMA
                                   // registers have been captured
  volatile uint32_t EPDATASTATUS;  // (0x46C) Provides information on which endpoint(s) an
                                   // acknowledged data transfer has occurred (EPDATA event)
  volatile uint32_t USBADDR;       // (0x470) Device USB address
  volatile uint32_t __RESERVED8[3];
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
  volatile uint32_t __RESERVED9[7];
  volatile uint32_t
      SIZE__ISOOUT;  // (0x4C0) Number of bytes received last on this ISO OUT data endpoint
  volatile uint32_t __RESERVED10[15];
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
  volatile uint32_t __RESERVED11[2];
  volatile uint32_t
      LOWPOWER;  // (0x52C) Controls USBD peripheral low power mode during USB suspend
  volatile uint32_t ISOINCONFIG;  // (0x530) Controls the response of the ISO IN endpoint to an
                                  // IN token when no data is ready to be sent
  volatile uint32_t __RESERVED12[33];
  volatile uint32_t EPIN_0__PTR;     // (0x600) Data pointer
  volatile uint32_t EPIN_0__MAXCNT;  // (0x604) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_0__AMOUNT;  // (0x608) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED13[2];
  volatile uint32_t EPIN_1__PTR;     // (0x614) Data pointer
  volatile uint32_t EPIN_1__MAXCNT;  // (0x618) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_1__AMOUNT;  // (0x61C) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED14[2];
  volatile uint32_t EPIN_2__PTR;     // (0x628) Data pointer
  volatile uint32_t EPIN_2__MAXCNT;  // (0x62C) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_2__AMOUNT;  // (0x630) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED15[2];
  volatile uint32_t EPIN_3__PTR;     // (0x63C) Data pointer
  volatile uint32_t EPIN_3__MAXCNT;  // (0x640) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_3__AMOUNT;  // (0x644) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED16[2];
  volatile uint32_t EPIN_4__PTR;     // (0x650) Data pointer
  volatile uint32_t EPIN_4__MAXCNT;  // (0x654) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_4__AMOUNT;  // (0x658) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED17[2];
  volatile uint32_t EPIN_5__PTR;     // (0x664) Data pointer
  volatile uint32_t EPIN_5__MAXCNT;  // (0x668) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_5__AMOUNT;  // (0x66C) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED18[2];
  volatile uint32_t EPIN_6__PTR;     // (0x678) Data pointer
  volatile uint32_t EPIN_6__MAXCNT;  // (0x67C) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_6__AMOUNT;  // (0x680) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED19[2];
  volatile uint32_t EPIN_7__PTR;     // (0x68C) Data pointer
  volatile uint32_t EPIN_7__MAXCNT;  // (0x690) Maximum number of bytes to transfer
  volatile uint32_t
      EPIN_7__AMOUNT;  // (0x694) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED20[2];
  volatile uint32_t ISOIN__PTR;     // (0x6A0) Data pointer
  volatile uint32_t ISOIN__MAXCNT;  // (0x6A4) Maximum number of bytes to transfer
  volatile uint32_t
      ISOIN__AMOUNT;  // (0x6A8) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED21[21];
  volatile uint32_t EPOUT_0__PTR;     // (0x700) Data pointer
  volatile uint32_t EPOUT_0__MAXCNT;  // (0x704) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_0__AMOUNT;  // (0x708) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED22[2];

  volatile uint32_t EPOUT_1__PTR;     // (0x714) Data pointer
  volatile uint32_t EPOUT_1__MAXCNT;  // (0x718) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_1__AMOUNT;  // (0x71C) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED23[2];
  volatile uint32_t EPOUT_2__PTR;     // (0x728) Data pointer
  volatile uint32_t EPOUT_2__MAXCNT;  // (0x72C) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_2__AMOUNT;  // (0x730) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED24[2];
  volatile uint32_t EPOUT_3__PTR;     // (0x73C) Data pointer
  volatile uint32_t EPOUT_3__MAXCNT;  // (0x740) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_3__AMOUNT;  // (0x744) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED25[2];
  volatile uint32_t EPOUT_4__PTR;     // (0x750) Data pointer
  volatile uint32_t EPOUT_4__MAXCNT;  // (0x754) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_4__AMOUNT;  // (0x758) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED26[2];
  volatile uint32_t EPOUT_5__PTR;     // (0x764) Data pointer
  volatile uint32_t EPOUT_5__MAXCNT;  // (0x768) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_5__AMOUNT;  // (0x76C) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED27[2];
  volatile uint32_t EPOUT_6__PTR;     // (0x778) Data pointer
  volatile uint32_t EPOUT_6__MAXCNT;  // (0x77C) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_6__AMOUNT;  // (0x780) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED28[2];
  volatile uint32_t EPOUT_7__PTR;     // (0x78C) Data pointer
  volatile uint32_t EPOUT_7__MAXCNT;  // (0x790) Maximum number of bytes to transfer
  volatile uint32_t
      EPOUT_7__AMOUNT;  // (0x794) Number of bytes transferred in the last transaction
  volatile uint32_t __RESERVED29[2];
  volatile uint32_t ISOOUT__PTR;     // (0x7A0) Data pointer
  volatile uint32_t ISOOUT__MAXCNT;  // (0x7A4) Maximum number of bytes to transfer
  volatile uint32_t
      ISOOUT__AMOUNT;  // (0x7A8) Number of bytes transferred in the last transaction
} usbd_t;

extern usbd_t* const USBD;

#endif  // USBD_H
