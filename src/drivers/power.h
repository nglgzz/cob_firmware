#ifndef POWER_H
#define POWER_H

#define POWER_BASE 0x40000000U

// Voltage supply detected on VBUS
#define POWER_EVENTS_USBDETECTED (*(volatile unsigned int *)(POWER_BASE + 0x11C))
// Voltage supply removed from VBUS
#define POWER_EVENTS_USBREMOVED (*(volatile unsigned int *)(POWER_BASE + 0x120))
// USB 3.3 V supply ready
#define POWER_EVENTS_USBPWRRDY (*(volatile unsigned int *)(POWER_BASE + 0x124))
// Enable interrupt
#define POWER_INTENSET (*(volatile unsigned int *)(POWER_BASE + 0x304))
// USB supply status
#define POWER_USBREGSTATUS (*(volatile unsigned int *)(POWER_BASE + 0x438))

#endif  // POWER_H
