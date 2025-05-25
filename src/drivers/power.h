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

// VBUSDETECT - VBUS input detection status (USBDETECTED and USBREMOVED events are derived from
// this information)
#define POWER_USBREGSTATUS_VBUSDETECT_Pos 0U
#define POWER_USBREGSTATUS_VBUSDETECT_Msk (0x1U << POWER_USBREGSTATUS_VBUSDETECT_Pos)
#define POWER_USBREGSTATUS_VBUSDETECT_NoVbus 0U
#define POWER_USBREGSTATUS_VBUSDETECT_VbusPresent 1U

// OUTPUTRDY - USB supply output settling time elapsed
#define POWER_USBREGSTATUS_OUTPUTRDY_Pos 1U
#define POWER_USBREGSTATUS_OUTPUTRDY_Msk (0x1U << POWER_USBREGSTATUS_OUTPUTRDY_Pos)
#define POWER_USBREGSTATUS_OUTPUTRDY_NotReady 0U
#define POWER_USBREGSTATUS_OUTPUTRDY_Ready 1U

/* Register: POWER_INTENSET */
/* Description: Enable interrupt */

/* Bit 9 : Write '1' to enable interrupt for event USBPWRRDY */
#define POWER_INTENSET_USBPWRRDY_Pos (9UL) /*!< Position of USBPWRRDY field. */
#define POWER_INTENSET_USBPWRRDY_Msk \
  (0x1UL << POWER_INTENSET_USBPWRRDY_Pos)         /*!< Bit mask of USBPWRRDY field. */
#define POWER_INTENSET_USBPWRRDY_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_USBPWRRDY_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_USBPWRRDY_Set (0x1UL)      /*!< Enable */

/* Bit 8 : Write '1' to enable interrupt for event USBREMOVED */
#define POWER_INTENSET_USBREMOVED_Pos (8UL) /*!< Position of USBREMOVED field. */
#define POWER_INTENSET_USBREMOVED_Msk \
  (0x1UL << POWER_INTENSET_USBREMOVED_Pos)         /*!< Bit mask of USBREMOVED field. */
#define POWER_INTENSET_USBREMOVED_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_USBREMOVED_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_USBREMOVED_Set (0x1UL)      /*!< Enable */

/* Bit 7 : Write '1' to enable interrupt for event USBDETECTED */
#define POWER_INTENSET_USBDETECTED_Pos (7UL) /*!< Position of USBDETECTED field. */
#define POWER_INTENSET_USBDETECTED_Msk \
  (0x1UL << POWER_INTENSET_USBDETECTED_Pos)         /*!< Bit mask of USBDETECTED field. */
#define POWER_INTENSET_USBDETECTED_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_USBDETECTED_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_USBDETECTED_Set (0x1UL)      /*!< Enable */

/* Bit 6 : Write '1' to enable interrupt for event SLEEPEXIT */
#define POWER_INTENSET_SLEEPEXIT_Pos (6UL) /*!< Position of SLEEPEXIT field. */
#define POWER_INTENSET_SLEEPEXIT_Msk \
  (0x1UL << POWER_INTENSET_SLEEPEXIT_Pos)         /*!< Bit mask of SLEEPEXIT field. */
#define POWER_INTENSET_SLEEPEXIT_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_SLEEPEXIT_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_SLEEPEXIT_Set (0x1UL)      /*!< Enable */

/* Bit 5 : Write '1' to enable interrupt for event SLEEPENTER */
#define POWER_INTENSET_SLEEPENTER_Pos (5UL) /*!< Position of SLEEPENTER field. */
#define POWER_INTENSET_SLEEPENTER_Msk \
  (0x1UL << POWER_INTENSET_SLEEPENTER_Pos)         /*!< Bit mask of SLEEPENTER field. */
#define POWER_INTENSET_SLEEPENTER_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_SLEEPENTER_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_SLEEPENTER_Set (0x1UL)      /*!< Enable */

/* Bit 2 : Write '1' to enable interrupt for event POFWARN */
#define POWER_INTENSET_POFWARN_Pos (2UL) /*!< Position of POFWARN field. */
#define POWER_INTENSET_POFWARN_Msk \
  (0x1UL << POWER_INTENSET_POFWARN_Pos)         /*!< Bit mask of POFWARN field. */
#define POWER_INTENSET_POFWARN_Disabled (0x0UL) /*!< Read: Disabled */
#define POWER_INTENSET_POFWARN_Enabled (0x1UL)  /*!< Read: Enabled */
#define POWER_INTENSET_POFWARN_Set (0x1UL)      /*!< Enable */

#endif  // POWER_H
