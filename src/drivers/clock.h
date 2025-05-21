#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#define CLOCK_BASE 0x40000000U

typedef struct {
  volatile uint32_t TASKS_HFCLKSTART;  // (0x000) Start HFXO crystal oscillator
  volatile uint32_t TASKS_HFCLKSTOP;   // (0x004) Stop HFXO crystal oscillator
  volatile uint32_t TASKS_LFCLKSTART;  // (0x008) Start LFCLK
  volatile uint32_t TASKS_LFCLKSTOP;   // (0x00C) Stop LFCLK
  volatile uint32_t TASKS_CAL;         // (0x010) Start calibration of LFRC
  volatile uint32_t TASKS_CTSTART;     // (0x014) Start calibration timer
  volatile uint32_t TASKS_CTSTOP;      // (0x018) Stop calibration timer
  volatile uint32_t __RESERVED[57];
  volatile uint32_t EVENTS_HFCLKSTARTED;  // (0x100) HFXO crystal oscillator started
  volatile uint32_t EVENTS_LFCLKSTARTED;  // (0x104) LFCLK started
  volatile uint32_t __RESERVED1;
  volatile uint32_t EVENTS_DONE;  // (0x10C) Calibration of LFRC completed
  volatile uint32_t EVENTS_CTTO;  // (0x110) Calibration timer timeout
  volatile uint32_t __RESERVED2[5];
  volatile uint32_t EVENTS_CTSTARTED;  // (0x128) Calibration timer has been started and is
                                       // ready to process new tasks
  volatile uint32_t EVENTS_CTSTOPPED;  // (0x12C) Calibration timer has been stopped and is
                                       // ready to process new tasks
  volatile uint32_t __RESERVED3[117];

  volatile uint32_t INTENSET;  // (0x304) Enable interrupt
  volatile uint32_t INTENCLR;  // (0x308) Disable interrupt
  volatile uint32_t __RESERVED4[63];

  volatile uint32_t
      HFCLKRUN;  // (0x408) Status indicating that HFCLKSTART task has been triggered
  volatile uint32_t HFCLKSTAT;  // (0x40C) HFCLK status
  volatile uint32_t __RESERVED5;
  volatile uint32_t
      LFCLKRUN;  // (0x414) Status indicating that LFCLKSTART task has been triggered
  volatile uint32_t LFCLKSTAT;     // (0x418) LFCLK status
  volatile uint32_t LFCLKSRCCOPY;  // (0x41C) Copy of LFCLKSRC register, set when LFCLKSTART
                                   // task was triggered
  volatile uint32_t __RESERVED6[62];

  volatile uint32_t LFCLKSRC;  // (0x518) Clock source for the LFCLK
  volatile uint32_t __RESERVED7[3];
  volatile uint32_t HFXODEBOUNCE;  // (0x528) HFXO debounce time. The HFXO is started by
                                   // triggering the TASKS_HFCLKSTART task.
  volatile uint32_t __RESERVED8[3];
  volatile uint32_t CTIV;  // (0x538) Calibration timer interval. This register is retained.
  volatile uint32_t __RESERVED9[8];
  volatile uint32_t
      TRACECONFIG;  // (0x55C) Clocking options for the trace port debug interface
  volatile uint32_t __RESERVED10[21];
  volatile uint32_t LFRCMODE;  // (0x5B4) LFRC mode configuration
} clock_t;

extern clock_t *const CLOCK;

/* Peripheral: CLOCK */
/* Description: Clock control */

/* Register: CLOCK_TASKS_HFCLKSTART */
/* Description: Start HFXO crystal oscillator */

/* Bit 0 : Start HFXO crystal oscillator */
#define CLOCK_TASKS_HFCLKSTART_TASKS_HFCLKSTART_Pos \
  (0UL) /*!< Position of TASKS_HFCLKSTART field. */
#define CLOCK_TASKS_HFCLKSTART_TASKS_HFCLKSTART_Msk                                        \
  (0x1UL << CLOCK_TASKS_HFCLKSTART_TASKS_HFCLKSTART_Pos) /*!< Bit mask of TASKS_HFCLKSTART \
                                                            field. */
#define CLOCK_TASKS_HFCLKSTART_TASKS_HFCLKSTART_Trigger (0x1UL) /*!< Trigger task */

/* Register: CLOCK_TASKS_HFCLKSTOP */
/* Description: Stop HFXO crystal oscillator */

/* Bit 0 : Stop HFXO crystal oscillator */
#define CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Pos \
  (0UL) /*!< Position of TASKS_HFCLKSTOP field. */
#define CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Msk \
  (0x1UL                                          \
   << CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Pos) /*!< Bit mask of TASKS_HFCLKSTOP field. */
#define CLOCK_TASKS_HFCLKSTOP_TASKS_HFCLKSTOP_Trigger (0x1UL) /*!< Trigger task */

/* Register: CLOCK_HFCLKSTAT */
/* Description: HFCLK status */

/* Bit 16 : HFCLK state */
#define CLOCK_HFCLKSTAT_STATE_Pos (16UL) /*!< Position of STATE field. */
#define CLOCK_HFCLKSTAT_STATE_Msk \
  (0x1UL << CLOCK_HFCLKSTAT_STATE_Pos)           /*!< Bit mask of STATE field. */
#define CLOCK_HFCLKSTAT_STATE_NotRunning (0x0UL) /*!< HFCLK not running */
#define CLOCK_HFCLKSTAT_STATE_Running (0x1UL)    /*!< HFCLK running */

/* Bit 0 : Source of HFCLK */
#define CLOCK_HFCLKSTAT_SRC_Pos (0UL) /*!< Position of SRC field. */
#define CLOCK_HFCLKSTAT_SRC_Msk \
  (0x1UL << CLOCK_HFCLKSTAT_SRC_Pos)     /*!< Bit mask of SRC field. */
#define CLOCK_HFCLKSTAT_SRC_RC (0x0UL)   /*!< 64 MHz internal oscillator (HFINT) */
#define CLOCK_HFCLKSTAT_SRC_Xtal (0x1UL) /*!< 64 MHz crystal oscillator (HFXO) */

/* Register: CLOCK_INTENSET */
/* Description: Enable interrupt */

/* Bit 11 : Write '1' to enable interrupt for event CTSTOPPED */
#define CLOCK_INTENSET_CTSTOPPED_Pos (11UL) /*!< Position of CTSTOPPED field. */
#define CLOCK_INTENSET_CTSTOPPED_Msk \
  (0x1UL << CLOCK_INTENSET_CTSTOPPED_Pos)         /*!< Bit mask of CTSTOPPED field. */
#define CLOCK_INTENSET_CTSTOPPED_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_CTSTOPPED_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_CTSTOPPED_Set (0x1UL)      /*!< Enable */

/* Bit 10 : Write '1' to enable interrupt for event CTSTARTED */
#define CLOCK_INTENSET_CTSTARTED_Pos (10UL) /*!< Position of CTSTARTED field. */
#define CLOCK_INTENSET_CTSTARTED_Msk \
  (0x1UL << CLOCK_INTENSET_CTSTARTED_Pos)         /*!< Bit mask of CTSTARTED field. */
#define CLOCK_INTENSET_CTSTARTED_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_CTSTARTED_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_CTSTARTED_Set (0x1UL)      /*!< Enable */

/* Bit 4 : Write '1' to enable interrupt for event CTTO */
#define CLOCK_INTENSET_CTTO_Pos (4UL) /*!< Position of CTTO field. */
#define CLOCK_INTENSET_CTTO_Msk \
  (0x1UL << CLOCK_INTENSET_CTTO_Pos)         /*!< Bit mask of CTTO field. */
#define CLOCK_INTENSET_CTTO_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_CTTO_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_CTTO_Set (0x1UL)      /*!< Enable */

/* Bit 3 : Write '1' to enable interrupt for event DONE */
#define CLOCK_INTENSET_DONE_Pos (3UL) /*!< Position of DONE field. */
#define CLOCK_INTENSET_DONE_Msk \
  (0x1UL << CLOCK_INTENSET_DONE_Pos)         /*!< Bit mask of DONE field. */
#define CLOCK_INTENSET_DONE_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_DONE_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_DONE_Set (0x1UL)      /*!< Enable */

/* Bit 1 : Write '1' to enable interrupt for event LFCLKSTARTED */
#define CLOCK_INTENSET_LFCLKSTARTED_Pos (1UL) /*!< Position of LFCLKSTARTED field. */
#define CLOCK_INTENSET_LFCLKSTARTED_Msk \
  (0x1UL << CLOCK_INTENSET_LFCLKSTARTED_Pos)         /*!< Bit mask of LFCLKSTARTED field. */
#define CLOCK_INTENSET_LFCLKSTARTED_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_LFCLKSTARTED_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_LFCLKSTARTED_Set (0x1UL)      /*!< Enable */

/* Bit 0 : Write '1' to enable interrupt for event HFCLKSTARTED */
#define CLOCK_INTENSET_HFCLKSTARTED_Pos (0UL) /*!< Position of HFCLKSTARTED field. */
#define CLOCK_INTENSET_HFCLKSTARTED_Msk \
  (0x1UL << CLOCK_INTENSET_HFCLKSTARTED_Pos)         /*!< Bit mask of HFCLKSTARTED field. */
#define CLOCK_INTENSET_HFCLKSTARTED_Disabled (0x0UL) /*!< Read: Disabled */
#define CLOCK_INTENSET_HFCLKSTARTED_Enabled (0x1UL)  /*!< Read: Enabled */
#define CLOCK_INTENSET_HFCLKSTARTED_Set (0x1UL)      /*!< Enable */

#endif  // CLOCK_H
