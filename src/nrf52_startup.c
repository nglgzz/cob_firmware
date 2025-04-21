#include <stdint.h>

// This is the startup file for the micro-controller, it does three things.
//
// 1. Creates a vector table for the MCU (located in the initial space of
//    the code memory, it's a list that maps interrupt handlers for
//    specific interrupt requests).
// 2. Initializes .data and .bss sections in RAM
// 3. Call main
//
// https://www.youtube.com/watch?v=2Hm8eEHsgls

// These are symbols created by the linker. Since they are added after
// compilation time, the compiler can't know about them. The extern keyword
// tells the compiler not to create a symbol for these variables (i.e. don't
// instantiate them), but instead assume they'll get instantiated elsewhere
// (i.e. during linking).
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _la_data;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _stack_top;

// the main function
int main(void);

// exception handlers
// __attribute__((weak, alias("name"))) declares a weak function, if you didn't
// provide a strong symbol name for that function, it will call the function you
// have aliased it to.
// https://stackoverflow.com/a/51970423
void Reset_Handler(void) __attribute__((weak, alias("Default_Handler")));
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemoryManagement_Handler(void)
    __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

void POWER_CLOCK_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));
void RADIO_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void UARTE0_UART0_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));
void SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));
void SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler(void)
    __attribute__((weak, alias("Default_Handler")));
void NFCT_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void GPIOTE_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));

typedef void (*vector_table_t)(void);
extern const vector_table_t _vectors[23];
const vector_table_t _vectors[23]
    // The section attribute tells the compiler to create a new section in the
    // object file called ".isr_vector" that will contain this variable. This is
    // to be able to place this variable manually during linking, because the
    // vector table should be at the beginning of the memory map, but
    // initialized variables by default are defined in the data section a while
    // later.
    //
    // The vector table is specified in part in the Cortex-M4 Devices Generic
    // User Guide (chapter 2.3.4 defines the common exceptions), and in part in
    // the datasheet (chapter 6.1.8).
    //
    // https://developer.arm.com/documentation/dui0553/latest
    // https://www.mouser.com/datasheet/2/297/nRF52840_OPS_v0.5-1074816.pdf
    // (chapter 45)

    __attribute__((used, section(".isr_vector"))) = {
        (vector_table_t)&_stack_top,
        /* Exceptions */
        Reset_Handler, NMI_Handler, HardFault_Handler, MemoryManagement_Handler,
        BusFault_Handler, UsageFault_Handler, 0, 0, 0, 0, SVC_Handler,
        DebugMon_Handler, 0, PendSV_Handler, SysTick_Handler,

        /* External Interrupts */
        POWER_CLOCK_IRQHandler, RADIO_IRQHandler, UARTE0_UART0_IRQHandler,
        SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler,
        SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler, NFCT_IRQHandler,
        GPIOTE_IRQHandler,
        // ...
};

void Default_Handler(void) {
  // Copy .data section from FLASH to RAM.
  uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
  uint8_t *pDst = (uint8_t *)&_sdata;    // RAM
  uint8_t *pSrc = (uint8_t *)&_la_data;  // Flash
  for (uint32_t i = 0; i < size; i++) {
    *pDst++ = *pSrc++;
  }

  // Initialize the .bss section to zero in RAM
  size = (uint32_t)&_ebss - (uint32_t)&_sbss;
  pDst = (uint8_t *)&_sbss;
  for (uint32_t i = 0; i < size; i++) {
    *pDst++ = 0;
  }

  main();
}