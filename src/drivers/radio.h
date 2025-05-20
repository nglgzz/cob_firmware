#ifndef RADIO_H

#include <stdint.h>

#define RADIO_BASE 0x40001000U

#define RADIO_PAYLOAD_MAXLEN 128
typedef struct {
  volatile uint8_t len;
  volatile uint8_t data[RADIO_PAYLOAD_MAXLEN];
} radio_packet_t;

void init_radio();
void radio_receive();
void radio_send(radio_packet_t *payload);

typedef struct {
  volatile uint32_t TASKS_TXEN;       // (0x000) Enable RADIO in TX mode
  volatile uint32_t TASKS_RXEN;       // (0x004) Enable RADIO in RX mode
  volatile uint32_t TASKS_START;      // (0x008) Start RADIO
  volatile uint32_t TASKS_STOP;       // (0x00C) Stop RADIO
  volatile uint32_t TASKS_DISABLE;    // (0x010) Disable RADIO
  volatile uint32_t TASKS_RSSISTART;  // (0x014) Start the RSSI and take one single sample of
                                      // the receive signal strength
  volatile uint32_t TASKS_RSSISTOP;   // (0x018) Stop the RSSI measurement
  volatile uint32_t TASKS_BCSTART;    // (0x01C) Start the bit counter
  volatile uint32_t TASKS_BCSTOP;     // (0x020) Stop the bit counter
  volatile uint32_t
      TASKS_EDSTART;  // (0x024) Start the energy detect measurement used in IEEE 802.15.4 mode
  volatile uint32_t TASKS_EDSTOP;  // (0x028) Stop the energy detect measurement

  volatile uint32_t
      TASKS_CCASTART;  // (0x02C) Start the clear channel assessment used in IEEE 802.15.4 mode
  volatile uint32_t TASKS_CCASTOP;  // (0x030) Stop the clear channel assessment

  volatile uint32_t __RESERVED[51];
  volatile uint32_t EVENTS_READY;    // (0x100) RADIO has ramped up and is ready to be started
  volatile uint32_t EVENTS_ADDRESS;  // (0x104) Address sent or received
  volatile uint32_t EVENTS_PAYLOAD;  // (0x108) Packet payload sent or received

  volatile uint32_t EVENTS_END;       // (0x10C) Packet sent or received
  volatile uint32_t EVENTS_DISABLED;  // (0x110) RADIO has been disabled
  volatile uint32_t
      EVENTS_DEVMATCH;  // (0x114) A device address match occurred on the last received packet
  volatile uint32_t
      EVENTS_DEVMISS;  // (0x118) No device address match occurred on the last received packet
  volatile uint32_t EVENTS_RSSIEND;  // (0x11C) Sampling of receive signal strength complete
  volatile uint32_t __RESERVED1[2];
  volatile uint32_t EVENTS_BCMATCH;  // (0x128) Bit counter reached bit count value
  volatile uint32_t __RESERVED2[1];
  volatile uint32_t EVENTS_CRCOK;       // (0x130) Packet received with CRC ok
  volatile uint32_t EVENTS_CRCERROR;    // (0x134) Packet received with CRC error
  volatile uint32_t EVENTS_FRAMESTART;  // (0x138) IEEE 802.15.4 length field received
  volatile uint32_t EVENTS_EDEND;  // (0x13C) Sampling of energy detection complete. A new ED
                                   // sample is ready for readout from the
  volatile uint32_t EVENTS_EDSTOPPED;  // (0x140) The sampling of energy detection has stopped
  volatile uint32_t EVENTS_CCAIDLE;    // (0x144) Wireless medium in idle - clear to send
  volatile uint32_t EVENTS_CCABUSY;    // (0x148) Wireless medium busy - do not send
  volatile uint32_t EVENTS_CCSTOPPED;  // (0x14C) The CCA has stopped
  volatile uint32_t EVENTS_RATEBOOST;  // (0x150) Ble_LR CI field received, receive mode is
                                       // changed from Ble_LR125Kbit to Ble_LR500Kbit.
  volatile uint32_t
      EVENTS_TXREADY;  // (0x154) RADIO has ramped up and is ready to be started TX path
  volatile uint32_t
      EVENTS_RXREADY;  // (0x158) RADIO has ramped up and is ready to be started RX path
  volatile uint32_t EVENTS_MHRMATCH;  // (0x15C) MAC header match found
  volatile uint32_t __RESERVED3[2];
  volatile uint32_t EVENTS_SYNC;    // (0x168) Preamble indicator.
  volatile uint32_t EVENTS_PHYEND;  // (0x16C) Generated in Ble_LR125Kbit, Ble_LR500Kbit and
                                    // Ieee802154_250Kbit modes when last bit is sent on
  volatile uint32_t __RESERVED4[36];
  volatile uint32_t SHORTS;  // (0x200) Shortcuts between local events and tasks

  volatile uint32_t __RESERVED5[64];
  volatile uint32_t INTENSET;  // (0x304) Enable interrupt
  volatile uint32_t INTENCLR;  // (0x308) Disable interrupt
  volatile uint32_t __RESERVED6[61];
  volatile uint32_t CRCSTATUS;  // (0x400) CRC status
  volatile uint32_t __RESERVED7;
  volatile uint32_t RXMATCH;  // (0x408) Received address
  volatile uint32_t RXCRC;    // (0x40C) CRC field of previously received packet
  volatile uint32_t DAI;      // (0x410) Device address match index
  volatile uint32_t PDUSTAT;  // (0x414) Payload status
  volatile uint32_t __RESERVED8[59];
  volatile uint32_t PACKETPTR;  // (0x504) Packet pointer
  volatile uint32_t FREQUENCY;  // (0x508) Frequency
  volatile uint32_t TXPOWER;    // (0x50C) Output power
  volatile uint32_t MODE;       // (0x510) Data rate and modulation
  volatile uint32_t PCNF0;      // (0x514) Packet configuration register 0
  volatile uint32_t PCNF1;      // (0x518) Packet configuration register 1
  volatile uint32_t BASE0;      // (0x51C) Base address 0
  volatile uint32_t BASE1;      // (0x520) Base address 1
  volatile uint32_t PREFIX0;    // (0x524) Prefixes bytes for logical addresses 0-3

  volatile uint32_t PREFIX1;  // (0x528) Prefixes bytes for logical addresses 4-7

  volatile uint32_t TXADDRESS;    // (0x52C) Transmit address select
  volatile uint32_t RXADDRESSES;  // (0x530) Receive address select
  volatile uint32_t CRCCNF;       // (0x534) CRC configuration
  volatile uint32_t CRCPOLY;      // (0x538) CRC polynomial
  volatile uint32_t CRCINIT;      // (0x53C) CRC initial value
  volatile uint32_t __RESERVED9[1];
  volatile uint32_t TIFS;        // (0x544) Interframe spacing in µs
  volatile uint32_t RSSISAMPLE;  // (0x548) RSSI sample
  volatile uint32_t __RESERVED10[1];
  volatile uint32_t STATE;        // (0x550) Current radio state
  volatile uint32_t DATAWHITEIV;  // (0x554) Data whitening initial value
  volatile uint32_t __RESERVED11[2];
  volatile uint32_t BCC;  // (0x560) Bit counter compare
  volatile uint32_t __RESERVED12[39];
  volatile uint32_t DAB[8];        // (0x600) Device address base segment
  volatile uint32_t DAP[8];        // (0x620) Device address prefix
  volatile uint32_t DACNF;         // (0x640) Device address match configuration
  volatile uint32_t MHRMATCHCONF;  // (0x644) Search pattern configuration
  volatile uint32_t MHRMATCHMAS;   // (0x648) Pattern mask
  volatile uint32_t __RESERVED13[1];
  volatile uint32_t MODECNF0;  // (0x650) Radio mode configuration register 0
  volatile uint32_t __RESERVED14[3];
  volatile uint32_t SFD;       // (0x660) IEEE 802.15.4 start of frame delimiter
  volatile uint32_t EDCNT;     // (0x664) IEEE 802.15.4 energy detect loop count
  volatile uint32_t EDSAMPLE;  // (0x668) IEEE 802.15.4 energy detect level
  volatile uint32_t CCACTRL;   // (0x66C) IEEE 802.15.4 clear channel assessment control
  volatile uint32_t __RESERVED15[611];
  volatile uint32_t POWER;  // (0xFFC) Peripheral power control
} radio_t;

extern radio_t *const RADIO;

/* Register: RADIO_TXPOWER */
/* Description: Output power */

/* Bits 7..0 : RADIO output power */
#define RADIO_TXPOWER_TXPOWER_Pos (0UL) /*!< Position of TXPOWER field. */
#define RADIO_TXPOWER_TXPOWER_Msk \
  (0xFFUL << RADIO_TXPOWER_TXPOWER_Pos)         /*!< Bit mask of TXPOWER field. */
#define RADIO_TXPOWER_TXPOWER_0dBm (0x00UL)     /*!< 0 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos2dBm (0x02UL)  /*!< +2 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos3dBm (0x03UL)  /*!< +3 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos4dBm (0x04UL)  /*!< +4 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos5dBm (0x05UL)  /*!< +5 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos6dBm (0x06UL)  /*!< +6 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos7dBm (0x07UL)  /*!< +7 dBm */
#define RADIO_TXPOWER_TXPOWER_Pos8dBm (0x08UL)  /*!< +8 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg40dBm (0xD8UL) /*!< -40 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg30dBm (0xE2UL) /*!< Deprecated enumerator -  -40 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg20dBm (0xECUL) /*!< -20 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg16dBm (0xF0UL) /*!< -16 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg12dBm (0xF4UL) /*!< -12 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg8dBm (0xF8UL)  /*!< -8 dBm */
#define RADIO_TXPOWER_TXPOWER_Neg4dBm (0xFCUL)  /*!< -4 dBm */

/* Register: RADIO_MODE */
/* Description: Data rate and modulation */

/* Bits 3..0 : Radio data rate and modulation setting. The radio supports
 * frequency-shift keying (FSK) modulation. */
#define RADIO_MODE_MODE_Pos (0UL)                          /*!< Position of MODE field. */
#define RADIO_MODE_MODE_Msk (0xFUL << RADIO_MODE_MODE_Pos) /*!< Bit mask of MODE field. */
#define RADIO_MODE_MODE_Nrf_1Mbit (0x0UL) /*!< 1 Mbit/s Nordic proprietary radio mode */
#define RADIO_MODE_MODE_Nrf_2Mbit (0x1UL) /*!< 2 Mbit/s Nordic proprietary radio mode */
#define RADIO_MODE_MODE_Ble_1Mbit (0x3UL) /*!< 1 Mbit/s BLE */
#define RADIO_MODE_MODE_Ble_2Mbit (0x4UL) /*!< 2 Mbit/s BLE */
#define RADIO_MODE_MODE_Ble_LR125Kbit \
  (0x5UL) /*!< Long range 125 kbit/s TX, 125 kbit/s and 500 kbit/s RX */
#define RADIO_MODE_MODE_Ble_LR500Kbit \
  (0x6UL) /*!< Long range 500 kbit/s TX, 125 kbit/s and 500 kbit/s RX */
#define RADIO_MODE_MODE_Ieee802154_250Kbit (0xFUL) /*!< IEEE 802.15.4-2006 250 kbit/s */

/* Register: RADIO_PCNF0 */
/* Description: Packet configuration register 0 */

/* Bits 30..29 : Length of TERM field in Long Range operation */
#define RADIO_PCNF0_TERMLEN_Pos (29UL) /*!< Position of TERMLEN field. */
#define RADIO_PCNF0_TERMLEN_Msk \
  (0x3UL << RADIO_PCNF0_TERMLEN_Pos) /*!< Bit mask of TERMLEN field. */

/* Bit 26 : Indicates if LENGTH field contains CRC or not */
#define RADIO_PCNF0_CRCINC_Pos (26UL) /*!< Position of CRCINC field. */
#define RADIO_PCNF0_CRCINC_Msk \
  (0x1UL << RADIO_PCNF0_CRCINC_Pos)        /*!< Bit mask of CRCINC field. */
#define RADIO_PCNF0_CRCINC_Exclude (0x0UL) /*!< LENGTH does not contain CRC */
#define RADIO_PCNF0_CRCINC_Include (0x1UL) /*!< LENGTH includes CRC */

/* Bits 25..24 : Length of preamble on air. Decision point: TASKS_START task */
#define RADIO_PCNF0_PLEN_Pos (24UL)                          /*!< Position of PLEN field. */
#define RADIO_PCNF0_PLEN_Msk (0x3UL << RADIO_PCNF0_PLEN_Pos) /*!< Bit mask of PLEN field. */
#define RADIO_PCNF0_PLEN_8bit (0x0UL)                        /*!< 8-bit preamble */
#define RADIO_PCNF0_PLEN_16bit (0x1UL)                       /*!< 16-bit preamble */
#define RADIO_PCNF0_PLEN_32bitZero \
  (0x2UL) /*!< 32-bit zero preamble - used for IEEE 802.15.4 */
#define RADIO_PCNF0_PLEN_LongRange (0x3UL) /*!< Preamble - used for BLE long range */

/* Bits 23..22 : Length of code indicator - long range */
#define RADIO_PCNF0_CILEN_Pos (22UL) /*!< Position of CILEN field. */
#define RADIO_PCNF0_CILEN_Msk                                    \
  (0x3UL << RADIO_PCNF0_CILEN_Pos) /*!< Bit mask of CILEN field. \
                                    */

/* Bit 20 : Include or exclude S1 field in RAM */
#define RADIO_PCNF0_S1INCL_Pos (20UL) /*!< Position of S1INCL field. */
#define RADIO_PCNF0_S1INCL_Msk \
  (0x1UL << RADIO_PCNF0_S1INCL_Pos) /*!< Bit mask of S1INCL field. */
#define RADIO_PCNF0_S1INCL_Automatic \
  (0x0UL) /*!< Include S1 field in RAM only if S1LEN &gt; 0 */
#define RADIO_PCNF0_S1INCL_Include \
  (0x1UL) /*!< Always include S1 field in RAM independent of S1LEN */

/* Bits 19..16 : Length on air of S1 field in number of bits. */
#define RADIO_PCNF0_S1LEN_Pos (16UL) /*!< Position of S1LEN field. */
#define RADIO_PCNF0_S1LEN_Msk                                    \
  (0xFUL << RADIO_PCNF0_S1LEN_Pos) /*!< Bit mask of S1LEN field. \
                                    */

/* Bit 8 : Length on air of S0 field in number of bytes. */
#define RADIO_PCNF0_S0LEN_Pos (8UL) /*!< Position of S0LEN field. */
#define RADIO_PCNF0_S0LEN_Msk                                    \
  (0x1UL << RADIO_PCNF0_S0LEN_Pos) /*!< Bit mask of S0LEN field. \
                                    */

/* Bits 3..0 : Length on air of LENGTH field in number of bits. */
#define RADIO_PCNF0_LFLEN_Pos (0UL) /*!< Position of LFLEN field. */
#define RADIO_PCNF0_LFLEN_Msk                                    \
  (0xFUL << RADIO_PCNF0_LFLEN_Pos) /*!< Bit mask of LFLEN field. \
                                    */

/* Register: RADIO_PCNF1 */
/* Description: Packet configuration register 1 */

/* Bit 25 : Enable or disable packet whitening */
#define RADIO_PCNF1_WHITEEN_Pos (25UL) /*!< Position of WHITEEN field. */
#define RADIO_PCNF1_WHITEEN_Msk \
  (0x1UL << RADIO_PCNF1_WHITEEN_Pos)         /*!< Bit mask of WHITEEN field. */
#define RADIO_PCNF1_WHITEEN_Disabled (0x0UL) /*!< Disable */
#define RADIO_PCNF1_WHITEEN_Enabled (0x1UL)  /*!< Enable */

/* Bit 24 : On air endianness of packet, this applies to the S0, LENGTH, S1 and the PAYLOAD
 * fields. */
#define RADIO_PCNF1_ENDIAN_Pos (24UL) /*!< Position of ENDIAN field. */
#define RADIO_PCNF1_ENDIAN_Msk \
  (0x1UL << RADIO_PCNF1_ENDIAN_Pos)       /*!< Bit mask of ENDIAN field. */
#define RADIO_PCNF1_ENDIAN_Little (0x0UL) /*!< Least significant bit on air first */
#define RADIO_PCNF1_ENDIAN_Big (0x1UL)    /*!< Most significant bit on air first */

/* Bits 18..16 : Base address length in number of bytes */
#define RADIO_PCNF1_BALEN_Pos (16UL) /*!< Position of BALEN field. */
#define RADIO_PCNF1_BALEN_Msk                                    \
  (0x7UL << RADIO_PCNF1_BALEN_Pos) /*!< Bit mask of BALEN field. \
                                    */

/* Bits 15..8 : Static length in number of bytes */
#define RADIO_PCNF1_STATLEN_Pos (8UL) /*!< Position of STATLEN field. */
#define RADIO_PCNF1_STATLEN_Msk \
  (0xFFUL << RADIO_PCNF1_STATLEN_Pos) /*!< Bit mask of STATLEN field. */

/* Bits 7..0 : Maximum length of packet payload. If the packet payload is larger than MAXLEN,
 * the radio will truncate the payload to MAXLEN. */
#define RADIO_PCNF1_MAXLEN_Pos (0UL) /*!< Position of MAXLEN field. */
#define RADIO_PCNF1_MAXLEN_Msk \
  (0xFFUL << RADIO_PCNF1_MAXLEN_Pos) /*!< Bit mask of MAXLEN field. */

/* Register: RADIO_CRCCNF */
/* Description: CRC configuration */

/* Bits 9..8 : Include or exclude packet address field out of CRC calculation. */
#define RADIO_CRCCNF_SKIPADDR_Pos (8UL) /*!< Position of SKIPADDR field. */
#define RADIO_CRCCNF_SKIPADDR_Msk \
  (0x3UL << RADIO_CRCCNF_SKIPADDR_Pos)        /*!< Bit mask of SKIPADDR field. */
#define RADIO_CRCCNF_SKIPADDR_Include (0x0UL) /*!< CRC calculation includes address field */
#define RADIO_CRCCNF_SKIPADDR_Skip                                                            \
  (0x1UL) /*!< CRC calculation does not include address field. The CRC calculation will start \
             at the first byte after the address. */
#define RADIO_CRCCNF_SKIPADDR_Ieee802154                                                     \
  (0x2UL) /*!< CRC calculation as per 802.15.4 standard. Starting at first byte after length \
             field. */

/* Bits 1..0 : CRC length in number of bytes. */
#define RADIO_CRCCNF_LEN_Pos (0UL)                           /*!< Position of LEN field. */
#define RADIO_CRCCNF_LEN_Msk (0x3UL << RADIO_CRCCNF_LEN_Pos) /*!< Bit mask of LEN field. */
#define RADIO_CRCCNF_LEN_Disabled \
  (0x0UL) /*!< CRC length is zero and CRC calculation is disabled */
#define RADIO_CRCCNF_LEN_One \
  (0x1UL) /*!< CRC length is one byte and CRC calculation is enabled */
#define RADIO_CRCCNF_LEN_Two \
  (0x2UL) /*!< CRC length is two bytes and CRC calculation is enabled */
#define RADIO_CRCCNF_LEN_Three \
  (0x3UL) /*!< CRC length is three bytes and CRC calculation is enabled */

/* Register: RADIO_CRCPOLY */
/* Description: CRC polynomial */

/* Bits 23..0 : CRC polynomial */
#define RADIO_CRCPOLY_CRCPOLY_Pos (0UL) /*!< Position of CRCPOLY field. */
#define RADIO_CRCPOLY_CRCPOLY_Msk \
  (0xFFFFFFUL << RADIO_CRCPOLY_CRCPOLY_Pos) /*!< Bit mask of CRCPOLY field. */

/* Register: RADIO_CRCINIT */
/* Description: CRC initial value */

/* Bits 23..0 : CRC initial value */
#define RADIO_CRCINIT_CRCINIT_Pos (0UL) /*!< Position of CRCINIT field. */
#define RADIO_CRCINIT_CRCINIT_Msk \
  (0xFFFFFFUL << RADIO_CRCINIT_CRCINIT_Pos) /*!< Bit mask of CRCINIT field. */

/* Register: RADIO_STATE */
/* Description: Current radio state */

/* Bits 3..0 : Current radio state */
#define RADIO_STATE_STATE_Pos (0UL) /*!< Position of STATE field. */
#define RADIO_STATE_STATE_Msk                                             \
  (0xFUL << RADIO_STATE_STATE_Pos)          /*!< Bit mask of STATE field. \
                                             */
#define RADIO_STATE_STATE_Disabled (0x0UL)  /*!< RADIO is in the Disabled state */
#define RADIO_STATE_STATE_RxRu (0x1UL)      /*!< RADIO is in the RXRU state */
#define RADIO_STATE_STATE_RxIdle (0x2UL)    /*!< RADIO is in the RXIDLE state */
#define RADIO_STATE_STATE_Rx (0x3UL)        /*!< RADIO is in the RX state */
#define RADIO_STATE_STATE_RxDisable (0x4UL) /*!< RADIO is in the RXDISABLED state */
#define RADIO_STATE_STATE_TxRu (0x9UL)      /*!< RADIO is in the TXRU state */
#define RADIO_STATE_STATE_TxIdle (0xAUL)    /*!< RADIO is in the TXIDLE state */
#define RADIO_STATE_STATE_Tx (0xBUL)        /*!< RADIO is in the TX state */
#define RADIO_STATE_STATE_TxDisable (0xCUL) /*!< RADIO is in the TXDISABLED state */

/* Register: RADIO_SHORTS */
/* Description: Shortcuts between local events and tasks */

/* Bit 21 : Shortcut between event PHYEND and task START */
#define RADIO_SHORTS_PHYEND_START_Pos (21UL) /*!< Position of PHYEND_START field. */
#define RADIO_SHORTS_PHYEND_START_Msk \
  (0x1UL << RADIO_SHORTS_PHYEND_START_Pos)         /*!< Bit mask of PHYEND_START field. */
#define RADIO_SHORTS_PHYEND_START_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_PHYEND_START_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 20 : Shortcut between event PHYEND and task DISABLE */
#define RADIO_SHORTS_PHYEND_DISABLE_Pos (20UL) /*!< Position of PHYEND_DISABLE field. */
#define RADIO_SHORTS_PHYEND_DISABLE_Msk \
  (0x1UL << RADIO_SHORTS_PHYEND_DISABLE_Pos)         /*!< Bit mask of PHYEND_DISABLE field. */
#define RADIO_SHORTS_PHYEND_DISABLE_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_PHYEND_DISABLE_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 19 : Shortcut between event RXREADY and task START */
#define RADIO_SHORTS_RXREADY_START_Pos (19UL) /*!< Position of RXREADY_START field. */
#define RADIO_SHORTS_RXREADY_START_Msk \
  (0x1UL << RADIO_SHORTS_RXREADY_START_Pos)         /*!< Bit mask of RXREADY_START field. */
#define RADIO_SHORTS_RXREADY_START_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_RXREADY_START_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 18 : Shortcut between event TXREADY and task START */
#define RADIO_SHORTS_TXREADY_START_Pos (18UL) /*!< Position of TXREADY_START field. */
#define RADIO_SHORTS_TXREADY_START_Msk \
  (0x1UL << RADIO_SHORTS_TXREADY_START_Pos)         /*!< Bit mask of TXREADY_START field. */
#define RADIO_SHORTS_TXREADY_START_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_TXREADY_START_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 17 : Shortcut between event CCAIDLE and task STOP */
#define RADIO_SHORTS_CCAIDLE_STOP_Pos (17UL) /*!< Position of CCAIDLE_STOP field. */
#define RADIO_SHORTS_CCAIDLE_STOP_Msk \
  (0x1UL << RADIO_SHORTS_CCAIDLE_STOP_Pos)         /*!< Bit mask of CCAIDLE_STOP field. */
#define RADIO_SHORTS_CCAIDLE_STOP_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_CCAIDLE_STOP_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 16 : Shortcut between event EDEND and task DISABLE */
#define RADIO_SHORTS_EDEND_DISABLE_Pos (16UL) /*!< Position of EDEND_DISABLE field. */
#define RADIO_SHORTS_EDEND_DISABLE_Msk \
  (0x1UL << RADIO_SHORTS_EDEND_DISABLE_Pos)         /*!< Bit mask of EDEND_DISABLE field. */
#define RADIO_SHORTS_EDEND_DISABLE_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_EDEND_DISABLE_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 15 : Shortcut between event READY and task EDSTART */
#define RADIO_SHORTS_READY_EDSTART_Pos (15UL) /*!< Position of READY_EDSTART field. */
#define RADIO_SHORTS_READY_EDSTART_Msk \
  (0x1UL << RADIO_SHORTS_READY_EDSTART_Pos)         /*!< Bit mask of READY_EDSTART field. */
#define RADIO_SHORTS_READY_EDSTART_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_READY_EDSTART_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 14 : Shortcut between event FRAMESTART and task BCSTART */
#define RADIO_SHORTS_FRAMESTART_BCSTART_Pos \
  (14UL) /*!< Position of FRAMESTART_BCSTART field. */
#define RADIO_SHORTS_FRAMESTART_BCSTART_Msk                                                 \
  (0x1UL << RADIO_SHORTS_FRAMESTART_BCSTART_Pos) /*!< Bit mask of FRAMESTART_BCSTART field. \
                                                  */
#define RADIO_SHORTS_FRAMESTART_BCSTART_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_FRAMESTART_BCSTART_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 13 : Shortcut between event CCABUSY and task DISABLE */
#define RADIO_SHORTS_CCABUSY_DISABLE_Pos (13UL) /*!< Position of CCABUSY_DISABLE field. */
#define RADIO_SHORTS_CCABUSY_DISABLE_Msk \
  (0x1UL << RADIO_SHORTS_CCABUSY_DISABLE_Pos) /*!< Bit mask of CCABUSY_DISABLE field. */
#define RADIO_SHORTS_CCABUSY_DISABLE_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_CCABUSY_DISABLE_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 12 : Shortcut between event CCAIDLE and task TXEN */
#define RADIO_SHORTS_CCAIDLE_TXEN_Pos (12UL) /*!< Position of CCAIDLE_TXEN field. */
#define RADIO_SHORTS_CCAIDLE_TXEN_Msk \
  (0x1UL << RADIO_SHORTS_CCAIDLE_TXEN_Pos)         /*!< Bit mask of CCAIDLE_TXEN field. */
#define RADIO_SHORTS_CCAIDLE_TXEN_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_CCAIDLE_TXEN_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 11 : Shortcut between event RXREADY and task CCASTART */
#define RADIO_SHORTS_RXREADY_CCASTART_Pos (11UL) /*!< Position of RXREADY_CCASTART field. */
#define RADIO_SHORTS_RXREADY_CCASTART_Msk \
  (0x1UL << RADIO_SHORTS_RXREADY_CCASTART_Pos) /*!< Bit mask of RXREADY_CCASTART field. */
#define RADIO_SHORTS_RXREADY_CCASTART_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_RXREADY_CCASTART_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 8 : Shortcut between event DISABLED and task RSSISTOP */
#define RADIO_SHORTS_DISABLED_RSSISTOP_Pos (8UL) /*!< Position of DISABLED_RSSISTOP field. */
#define RADIO_SHORTS_DISABLED_RSSISTOP_Msk \
  (0x1UL << RADIO_SHORTS_DISABLED_RSSISTOP_Pos) /*!< Bit mask of DISABLED_RSSISTOP field. */
#define RADIO_SHORTS_DISABLED_RSSISTOP_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_DISABLED_RSSISTOP_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 6 : Shortcut between event ADDRESS and task BCSTART */
#define RADIO_SHORTS_ADDRESS_BCSTART_Pos (6UL) /*!< Position of ADDRESS_BCSTART field. */
#define RADIO_SHORTS_ADDRESS_BCSTART_Msk \
  (0x1UL << RADIO_SHORTS_ADDRESS_BCSTART_Pos) /*!< Bit mask of ADDRESS_BCSTART field. */
#define RADIO_SHORTS_ADDRESS_BCSTART_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_ADDRESS_BCSTART_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 5 : Shortcut between event END and task START */
#define RADIO_SHORTS_END_START_Pos (5UL) /*!< Position of END_START field. */
#define RADIO_SHORTS_END_START_Msk \
  (0x1UL << RADIO_SHORTS_END_START_Pos)         /*!< Bit mask of END_START field. */
#define RADIO_SHORTS_END_START_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_END_START_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 4 : Shortcut between event ADDRESS and task RSSISTART */
#define RADIO_SHORTS_ADDRESS_RSSISTART_Pos (4UL) /*!< Position of ADDRESS_RSSISTART field. */
#define RADIO_SHORTS_ADDRESS_RSSISTART_Msk \
  (0x1UL << RADIO_SHORTS_ADDRESS_RSSISTART_Pos) /*!< Bit mask of ADDRESS_RSSISTART field. */
#define RADIO_SHORTS_ADDRESS_RSSISTART_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_ADDRESS_RSSISTART_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 3 : Shortcut between event DISABLED and task RXEN */
#define RADIO_SHORTS_DISABLED_RXEN_Pos (3UL) /*!< Position of DISABLED_RXEN field. */
#define RADIO_SHORTS_DISABLED_RXEN_Msk \
  (0x1UL << RADIO_SHORTS_DISABLED_RXEN_Pos)         /*!< Bit mask of DISABLED_RXEN field. */
#define RADIO_SHORTS_DISABLED_RXEN_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_DISABLED_RXEN_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 2 : Shortcut between event DISABLED and task TXEN */
#define RADIO_SHORTS_DISABLED_TXEN_Pos (2UL) /*!< Position of DISABLED_TXEN field. */
#define RADIO_SHORTS_DISABLED_TXEN_Msk \
  (0x1UL << RADIO_SHORTS_DISABLED_TXEN_Pos)         /*!< Bit mask of DISABLED_TXEN field. */
#define RADIO_SHORTS_DISABLED_TXEN_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_DISABLED_TXEN_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 1 : Shortcut between event END and task DISABLE */
#define RADIO_SHORTS_END_DISABLE_Pos (1UL) /*!< Position of END_DISABLE field. */
#define RADIO_SHORTS_END_DISABLE_Msk \
  (0x1UL << RADIO_SHORTS_END_DISABLE_Pos)         /*!< Bit mask of END_DISABLE field. */
#define RADIO_SHORTS_END_DISABLE_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_END_DISABLE_Enabled (0x1UL)  /*!< Enable shortcut */

/* Bit 0 : Shortcut between event READY and task START */
#define RADIO_SHORTS_READY_START_Pos (0UL) /*!< Position of READY_START field. */
#define RADIO_SHORTS_READY_START_Msk \
  (0x1UL << RADIO_SHORTS_READY_START_Pos)         /*!< Bit mask of READY_START field. */
#define RADIO_SHORTS_READY_START_Disabled (0x0UL) /*!< Disable shortcut */
#define RADIO_SHORTS_READY_START_Enabled (0x1UL)  /*!< Enable shortcut */

/* Register: RADIO_INTENSET */
/* Description: Enable interrupt */

/* Bit 27 : Write '1' to enable interrupt for event PHYEND */
#define RADIO_INTENSET_PHYEND_Pos (27UL) /*!< Position of PHYEND field. */
#define RADIO_INTENSET_PHYEND_Msk \
  (0x1UL << RADIO_INTENSET_PHYEND_Pos)         /*!< Bit mask of PHYEND field. */
#define RADIO_INTENSET_PHYEND_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_PHYEND_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_PHYEND_Set (0x1UL)      /*!< Enable */

/* Bit 26 : Write '1' to enable interrupt for event SYNC */
#define RADIO_INTENSET_SYNC_Pos (26UL) /*!< Position of SYNC field. */
#define RADIO_INTENSET_SYNC_Msk \
  (0x1UL << RADIO_INTENSET_SYNC_Pos)         /*!< Bit mask of SYNC field. */
#define RADIO_INTENSET_SYNC_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_SYNC_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_SYNC_Set (0x1UL)      /*!< Enable */

/* Bit 23 : Write '1' to enable interrupt for event MHRMATCH */
#define RADIO_INTENSET_MHRMATCH_Pos (23UL) /*!< Position of MHRMATCH field. */
#define RADIO_INTENSET_MHRMATCH_Msk \
  (0x1UL << RADIO_INTENSET_MHRMATCH_Pos)         /*!< Bit mask of MHRMATCH field. */
#define RADIO_INTENSET_MHRMATCH_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_MHRMATCH_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_MHRMATCH_Set (0x1UL)      /*!< Enable */

/* Bit 22 : Write '1' to enable interrupt for event RXREADY */
#define RADIO_INTENSET_RXREADY_Pos (22UL) /*!< Position of RXREADY field. */
#define RADIO_INTENSET_RXREADY_Msk \
  (0x1UL << RADIO_INTENSET_RXREADY_Pos)         /*!< Bit mask of RXREADY field. */
#define RADIO_INTENSET_RXREADY_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_RXREADY_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_RXREADY_Set (0x1UL)      /*!< Enable */

/* Bit 21 : Write '1' to enable interrupt for event TXREADY */
#define RADIO_INTENSET_TXREADY_Pos (21UL) /*!< Position of TXREADY field. */
#define RADIO_INTENSET_TXREADY_Msk \
  (0x1UL << RADIO_INTENSET_TXREADY_Pos)         /*!< Bit mask of TXREADY field. */
#define RADIO_INTENSET_TXREADY_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_TXREADY_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_TXREADY_Set (0x1UL)      /*!< Enable */

/* Bit 20 : Write '1' to enable interrupt for event RATEBOOST */
#define RADIO_INTENSET_RATEBOOST_Pos (20UL) /*!< Position of RATEBOOST field. */
#define RADIO_INTENSET_RATEBOOST_Msk \
  (0x1UL << RADIO_INTENSET_RATEBOOST_Pos)         /*!< Bit mask of RATEBOOST field. */
#define RADIO_INTENSET_RATEBOOST_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_RATEBOOST_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_RATEBOOST_Set (0x1UL)      /*!< Enable */

/* Bit 19 : Write '1' to enable interrupt for event CCASTOPPED */
#define RADIO_INTENSET_CCASTOPPED_Pos (19UL) /*!< Position of CCASTOPPED field. */
#define RADIO_INTENSET_CCASTOPPED_Msk \
  (0x1UL << RADIO_INTENSET_CCASTOPPED_Pos)         /*!< Bit mask of CCASTOPPED field. */
#define RADIO_INTENSET_CCASTOPPED_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_CCASTOPPED_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_CCASTOPPED_Set (0x1UL)      /*!< Enable */

/* Bit 18 : Write '1' to enable interrupt for event CCABUSY */
#define RADIO_INTENSET_CCABUSY_Pos (18UL) /*!< Position of CCABUSY field. */
#define RADIO_INTENSET_CCABUSY_Msk \
  (0x1UL << RADIO_INTENSET_CCABUSY_Pos)         /*!< Bit mask of CCABUSY field. */
#define RADIO_INTENSET_CCABUSY_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_CCABUSY_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_CCABUSY_Set (0x1UL)      /*!< Enable */

/* Bit 17 : Write '1' to enable interrupt for event CCAIDLE */
#define RADIO_INTENSET_CCAIDLE_Pos (17UL) /*!< Position of CCAIDLE field. */
#define RADIO_INTENSET_CCAIDLE_Msk \
  (0x1UL << RADIO_INTENSET_CCAIDLE_Pos)         /*!< Bit mask of CCAIDLE field. */
#define RADIO_INTENSET_CCAIDLE_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_CCAIDLE_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_CCAIDLE_Set (0x1UL)      /*!< Enable */

/* Bit 16 : Write '1' to enable interrupt for event EDSTOPPED */
#define RADIO_INTENSET_EDSTOPPED_Pos (16UL) /*!< Position of EDSTOPPED field. */
#define RADIO_INTENSET_EDSTOPPED_Msk \
  (0x1UL << RADIO_INTENSET_EDSTOPPED_Pos)         /*!< Bit mask of EDSTOPPED field. */
#define RADIO_INTENSET_EDSTOPPED_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_EDSTOPPED_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_EDSTOPPED_Set (0x1UL)      /*!< Enable */

/* Bit 15 : Write '1' to enable interrupt for event EDEND */
#define RADIO_INTENSET_EDEND_Pos (15UL) /*!< Position of EDEND field. */
#define RADIO_INTENSET_EDEND_Msk \
  (0x1UL << RADIO_INTENSET_EDEND_Pos)         /*!< Bit mask of EDEND field. */
#define RADIO_INTENSET_EDEND_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_EDEND_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_EDEND_Set (0x1UL)      /*!< Enable */

/* Bit 14 : Write '1' to enable interrupt for event FRAMESTART */
#define RADIO_INTENSET_FRAMESTART_Pos (14UL) /*!< Position of FRAMESTART field. */
#define RADIO_INTENSET_FRAMESTART_Msk \
  (0x1UL << RADIO_INTENSET_FRAMESTART_Pos)         /*!< Bit mask of FRAMESTART field. */
#define RADIO_INTENSET_FRAMESTART_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_FRAMESTART_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_FRAMESTART_Set (0x1UL)      /*!< Enable */

/* Bit 13 : Write '1' to enable interrupt for event CRCERROR */
#define RADIO_INTENSET_CRCERROR_Pos (13UL) /*!< Position of CRCERROR field. */
#define RADIO_INTENSET_CRCERROR_Msk \
  (0x1UL << RADIO_INTENSET_CRCERROR_Pos)         /*!< Bit mask of CRCERROR field. */
#define RADIO_INTENSET_CRCERROR_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_CRCERROR_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_CRCERROR_Set (0x1UL)      /*!< Enable */

/* Bit 12 : Write '1' to enable interrupt for event CRCOK */
#define RADIO_INTENSET_CRCOK_Pos (12UL) /*!< Position of CRCOK field. */
#define RADIO_INTENSET_CRCOK_Msk \
  (0x1UL << RADIO_INTENSET_CRCOK_Pos)         /*!< Bit mask of CRCOK field. */
#define RADIO_INTENSET_CRCOK_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_CRCOK_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_CRCOK_Set (0x1UL)      /*!< Enable */

/* Bit 10 : Write '1' to enable interrupt for event BCMATCH */
#define RADIO_INTENSET_BCMATCH_Pos (10UL) /*!< Position of BCMATCH field. */
#define RADIO_INTENSET_BCMATCH_Msk \
  (0x1UL << RADIO_INTENSET_BCMATCH_Pos)         /*!< Bit mask of BCMATCH field. */
#define RADIO_INTENSET_BCMATCH_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_BCMATCH_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_BCMATCH_Set (0x1UL)      /*!< Enable */

/* Bit 7 : Write '1' to enable interrupt for event RSSIEND */
#define RADIO_INTENSET_RSSIEND_Pos (7UL) /*!< Position of RSSIEND field. */
#define RADIO_INTENSET_RSSIEND_Msk \
  (0x1UL << RADIO_INTENSET_RSSIEND_Pos)         /*!< Bit mask of RSSIEND field. */
#define RADIO_INTENSET_RSSIEND_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_RSSIEND_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_RSSIEND_Set (0x1UL)      /*!< Enable */

/* Bit 6 : Write '1' to enable interrupt for event DEVMISS */
#define RADIO_INTENSET_DEVMISS_Pos (6UL) /*!< Position of DEVMISS field. */
#define RADIO_INTENSET_DEVMISS_Msk \
  (0x1UL << RADIO_INTENSET_DEVMISS_Pos)         /*!< Bit mask of DEVMISS field. */
#define RADIO_INTENSET_DEVMISS_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_DEVMISS_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_DEVMISS_Set (0x1UL)      /*!< Enable */

/* Bit 5 : Write '1' to enable interrupt for event DEVMATCH */
#define RADIO_INTENSET_DEVMATCH_Pos (5UL) /*!< Position of DEVMATCH field. */
#define RADIO_INTENSET_DEVMATCH_Msk \
  (0x1UL << RADIO_INTENSET_DEVMATCH_Pos)         /*!< Bit mask of DEVMATCH field. */
#define RADIO_INTENSET_DEVMATCH_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_DEVMATCH_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_DEVMATCH_Set (0x1UL)      /*!< Enable */

/* Bit 4 : Write '1' to enable interrupt for event DISABLED */
#define RADIO_INTENSET_DISABLED_Pos (4UL) /*!< Position of DISABLED field. */
#define RADIO_INTENSET_DISABLED_Msk \
  (0x1UL << RADIO_INTENSET_DISABLED_Pos)         /*!< Bit mask of DISABLED field. */
#define RADIO_INTENSET_DISABLED_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_DISABLED_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_DISABLED_Set (0x1UL)      /*!< Enable */

/* Bit 3 : Write '1' to enable interrupt for event END */
#define RADIO_INTENSET_END_Pos (3UL) /*!< Position of END field. */
#define RADIO_INTENSET_END_Msk                                          \
  (0x1UL << RADIO_INTENSET_END_Pos)         /*!< Bit mask of END field. \
                                             */
#define RADIO_INTENSET_END_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_END_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_END_Set (0x1UL)      /*!< Enable */

/* Bit 2 : Write '1' to enable interrupt for event PAYLOAD */
#define RADIO_INTENSET_PAYLOAD_Pos (2UL) /*!< Position of PAYLOAD field. */
#define RADIO_INTENSET_PAYLOAD_Msk \
  (0x1UL << RADIO_INTENSET_PAYLOAD_Pos)         /*!< Bit mask of PAYLOAD field. */
#define RADIO_INTENSET_PAYLOAD_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_PAYLOAD_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_PAYLOAD_Set (0x1UL)      /*!< Enable */

/* Bit 1 : Write '1' to enable interrupt for event ADDRESS */
#define RADIO_INTENSET_ADDRESS_Pos (1UL) /*!< Position of ADDRESS field. */
#define RADIO_INTENSET_ADDRESS_Msk \
  (0x1UL << RADIO_INTENSET_ADDRESS_Pos)         /*!< Bit mask of ADDRESS field. */
#define RADIO_INTENSET_ADDRESS_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_ADDRESS_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_ADDRESS_Set (0x1UL)      /*!< Enable */

/* Bit 0 : Write '1' to enable interrupt for event READY */
#define RADIO_INTENSET_READY_Pos (0UL) /*!< Position of READY field. */
#define RADIO_INTENSET_READY_Msk \
  (0x1UL << RADIO_INTENSET_READY_Pos)         /*!< Bit mask of READY field. */
#define RADIO_INTENSET_READY_Disabled (0x0UL) /*!< Read: Disabled */
#define RADIO_INTENSET_READY_Enabled (0x1UL)  /*!< Read: Enabled */
#define RADIO_INTENSET_READY_Set (0x1UL)      /*!< Enable */

#define RADIO_H
#endif  // RADIO_H
