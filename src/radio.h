#ifndef RADIO_H

#include <stdint.h>

#define RADIO_BASE 0x40001000U

struct radio {
  /* (0x000) Enable RADIO in TX mode */
  volatile uint32_t TASKS_TXEN;
  /* (0x004) Enable RADIO in RX mode */
  volatile uint32_t TASKS_RXEN;
  /* (0x008) Start RADIO */
  volatile uint32_t TASKS_START;
  /* (0x00C) Stop RADIO */
  volatile uint32_t TASKS_STOP;
  /* (0x010) Disable RADIO */
  volatile uint32_t TASKS_DISABLE;
  /* (0x014) Start the RSSI and take one single sample of the receive signal
   * strength */
  volatile uint32_t TASKS_RSSISTART;
  /* (0x018) Stop the RSSI measurement */
  volatile uint32_t TASKS_RSSISTOP;
  /* (0x01C) Start the bit counter */
  volatile uint32_t TASKS_BCSTART;
  /* (0x020) Stop the bit counter */
  volatile uint32_t TASKS_BCSTOP;
  /* (0x024) Start the energy detect measurement used in IEEE 802.15.4 mode */
  volatile uint32_t TASKS_EDSTART;
  /* (0x028) Stop the energy detect measurement */
  volatile uint32_t TASKS_EDSTOP;
  /* (0x02C) Start the clear channel assessment used in IEEE 802.15.4 mode */
  volatile uint32_t TASKS_CCASTART;
  /* (0x030) Stop the clear channel assessment */
  volatile uint32_t TASKS_CCASTOP;
  volatile uint32_t __RESERVED[52];
  /* (0x100) RADIO has ramped up and is ready to be started */
  volatile uint32_t EVENTS_READY;
  /* (0x104) Address sent or received */
  volatile uint32_t EVENTS_ADDRESS;
  /* (0x108) Packet payload sent or received */
  volatile uint32_t EVENTS_PAYLOAD;
  /* (0x10C) Packet sent or received */
  volatile uint32_t EVENTS_END;
  /* (0x110) RADIO has been disabled */
  volatile uint32_t EVENTS_DISABLED;
  /* (0x114) A device address match occurred on the last received packet */
  volatile uint32_t EVENTS_DEVMATCH;
  /* (0x118) No device address match occurred on the last received packet */
  volatile uint32_t EVENTS_DEVMISS;
  /* (0x11C) Sampling of receive signal strength complete */
  volatile uint32_t EVENTS_RSSIEND;
  volatile uint32_t __RESERVED1[3];
  /* (0x128) Bit counter reached bit count value */
  volatile uint32_t EVENTS_BCMATCH;
  volatile uint32_t __RESERVED2[2];
  /* (0x130) Packet received with CRC ok */
  volatile uint32_t EVENTS_CRCOK;
  /* (0x134) Packet received with CRC error */
  volatile uint32_t EVENTS_CRCERROR;
  /* (0x138) IEEE 802.15.4 length field received */
  volatile uint32_t EVENTS_FRAMESTART;
  /* (0x13C) Sampling of energy detection complete. A new ED sample is ready for
   * readout from the */
  volatile uint32_t EVENTS_EDEND;
  /* (0x140) The sampling of energy detection has stopped */
  volatile uint32_t EVENTS_EDSTOPPED;
  /* (0x144) Wireless medium in idle - clear to send */
  volatile uint32_t EVENTS_CCAIDLE;
  /* (0x148) Wireless medium busy - do not send */
  volatile uint32_t EVENTS_CCABUSY;
  /* (0x14C) The CCA has stopped */
  volatile uint32_t EVENTS_CCSTOPPED;
  /* (0x150) Ble_LR CI field received, receive mode is changed from
   * Ble_LR125Kbit to Ble_LR500Kbit. */
  volatile uint32_t EVENTS_RATEBOOST;
  /* (0x154) RADIO has ramped up and is ready to be started TX path */
  volatile uint32_t EVENTS_TXREADY;
  /* (0x158) RADIO has ramped up and is ready to be started RX path */
  volatile uint32_t EVENTS_RXREADY;
  /* (0x15C) MAC header match found */
  volatile uint32_t EVENTS_MHRMATCH;
  volatile uint32_t __RESERVED3[3];
  /* (0x168) Preamble indicator. */
  volatile uint32_t EVENTS_SYNC;
  /* (0x16C) Generated in Ble_LR125Kbit, Ble_LR500Kbit and Ieee802154_250Kbit
   * modes when last bit is sent on */
  volatile uint32_t EVENTS_PHYEND;
  volatile uint32_t __RESERVED4[37];
  /* (0x200) Shortcuts between local events and tasks */
  volatile uint32_t SHORTS;
  volatile uint32_t __RESERVED5[65];
  /* (0x304) Enable interrupt */
  volatile uint32_t INTENSET;
  /* (0x308) Disable interrupt */
  volatile uint32_t INTENCLR;
  volatile uint32_t __RESERVED6[62];
  /* (0x400) CRC status */
  volatile uint32_t CRCSTATUS;
  /* (0x408) Received address */
  volatile uint32_t RXMATCH;
  /* (0x40C) CRC field of previously received packet */
  volatile uint32_t RXCRC;
  /* (0x410) Device address match index */
  volatile uint32_t DAI;
  /* (0x414) Payload status */
  volatile uint32_t PDUSTAT;
  volatile uint32_t __RESERVED7[60];
  /* (0x504) Packet pointer */
  volatile uint32_t PACKETPTR;
  /* (0x508) Frequency */
  volatile uint32_t FREQUENCY;
  /* (0x50C) Output power */
  volatile uint32_t TXPOWER;
  /* (0x510) Data rate and modulation */
  volatile uint32_t MODE;
  /* (0x514) Packet configuration register 0 */
  volatile uint32_t PCNF0;
  /* (0x518) Packet configuration register 1 */
  volatile uint32_t PCNF1;
  /* (0x51C) Base address 0 */
  volatile uint32_t BASE0;
  /* (0x520) Base address 1 */
  volatile uint32_t BASE1;
  /* (0x524) Prefixes bytes for logical addresses 0-3 */
  volatile uint32_t PREFIX0;
  /* (0x528) Prefixes bytes for logical addresses 4-7 */
  volatile uint32_t PREFIX1;
  /* (0x52C) Transmit address select */
  volatile uint32_t TXADDRESS;
  /* (0x530) Receive address select */
  volatile uint32_t RXADDRESSES;
  /* (0x534) CRC configuration */
  volatile uint32_t CRCCNF;
  /* (0x538) CRC polynomial */
  volatile uint32_t CRCPOLY;
  /* (0x53C) CRC initial value */
  volatile uint32_t CRCINIT;
  volatile uint32_t __RESERVED8[2];
  /* (0x544) Interframe spacing in µs */
  volatile uint32_t TIFS;
  /* (0x548) RSSI sample */
  volatile uint32_t RSSISAMPLE;
  volatile uint32_t __RESERVED9[2];
  /* (0x550) Current radio state */
  volatile uint32_t STATE;
  /* (0x554) Data whitening initial value */
  volatile uint32_t DATAWHITEIV;
  volatile uint32_t __RESERVED10[3];
  /* (0x560) Bit counter compare */
  volatile uint32_t BCC;
  volatile uint32_t __RESERVED11[40];
  /* (0x600) Device address base segment */
  volatile uint32_t DAB[8];
  /* (0x620) Device address prefix  */
  volatile uint32_t DAP[8];
  /* (0x640) Device address match configuration */
  volatile uint32_t DACNF;
  /* (0x644) Search pattern configuration */
  volatile uint32_t MHRMATCHCONF;
  /* (0x648) Pattern mask */
  volatile uint32_t MHRMATCHMAS;
  volatile uint32_t __RESERVED12[2];
  /* (0x650) Radio mode configuration register 0 */
  volatile uint32_t MODECNF0;
  volatile uint32_t __RESERVED13[4];
  /* (0x660) IEEE 802.15.4 start of frame delimiter */
  volatile uint32_t SFD;
  /* (0x664) IEEE 802.15.4 energy detect loop count */
  volatile uint32_t EDCNT;
  /* (0x668) IEEE 802.15.4 energy detect level */
  volatile uint32_t EDSAMPLE;
  /* (0x66C) IEEE 802.15.4 clear channel assessment control */
  volatile uint32_t CCACTRL;
  volatile uint32_t __RESERVED14[612];
  /* (0xFFC) Peripheral power control */
  volatile uint32_t POWER;
};

extern struct radio *RADIO;

#define RADIO_H
#endif  // RADIO_H
