#include "core.h"

typedef struct {
  // Interrupt set enabled register
  // The bit position represents the ID of the interrupt to enable, with 0 being the handler on
  // position 16 in the vector table (i.e. the first external IRQ handler).
  volatile uint32_t ISER[8];
  volatile uint32_t __RESERVED[24];
  // Interrupt clear enabled register
  volatile uint32_t ICER[8];
  volatile uint32_t __RESERVED1[24];
  // Interrupt set pending register
  volatile uint32_t ISPR[8];
  volatile uint32_t __RESERVED2[24];
  // Interrupt clear pending register
  volatile uint32_t ICPR[8];
  volatile uint32_t __RESERVED3[24];
  // Interrupt active bit register
  volatile uint32_t IABR[8];
  volatile uint32_t __RESERVED4[56];
  // Interrupt priority register
  // Priority is 8 bits in the cortex m4 specification, but the Nordic implementation uses
  // values from 0 (highest) to 7 (lowest).
  volatile uint8_t IPR[240];
} nvic_t;

#define NVIC_BASE 0xE000E100U
nvic_t* const NVIC = ((nvic_t*)NVIC_BASE);

inline void NVIC_EnableIRQ(IRQn_t IRQn) { NVIC->ISER[IRQn >> 5U] |= (1 << (IRQn % 32U)); }

inline void NVIC_DisableIRQ(IRQn_t IRQn) { NVIC->ICER[IRQn >> 5U] |= (1 << (IRQn % 32U)); }

// Priority goes from 0 (highest) to 7 (lowest)
inline void NVIC_SetPriority(IRQn_t IRQn, uint8_t priority) {
  NVIC->IPR[(uint32_t)IRQn] = (uint8_t)(priority & 0xFFU);
}
