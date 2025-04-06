/**
 * https://www.youtube.com/watch?v=B7oKdUvRhQQ
 *
 * MEMORY defines which memory types we're using and their
 * permissions, origin, and length. This is used to check
 * that the executable fits in memory.
 */
MEMORY
{
    /* NOTE K = KiBi = 1024 bytes */
    FLASH(rx) : ORIGIN = 0x00000000, LENGTH = 256K
    RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/**
 * SECTIONS explains what parts of the object files are added
 * to the executable and in which order.
 *
 * Memory layout of FLASH (in order) is (TODO: source?):
 *       - [start]
 *       - vector table (.isr_vector)
 *       - code instructions (.text)
 *       - read only data (.rodata)
 *       - initialized global and static variables (.data)
 *       - unused code memory
 *
 * Memory layout of RAM (in order):
 *       - [start]
 *       - initialized global and static variables (.data,
 *               copied from FLASH)
 *       - uninitialized global and static variables (.bss)
 *       - heap
 *       - unused RAM
 *       - stack
 */
SECTIONS
{
    /**
     * This is just a label, it can be anything, though
     * there are conventions on the naming
     */
    .text :
    {
        /**
         * this is a wildcard getting all .isr_vector
         * sections from all input files.
         */
        *(.isr_vector) /* Vector table */
        *(.text)
        /* *(.rodata) */

    /**
     * Defines where this section is stored.
     * Syntax is `> VMA AT > LMA`, VMA is virtual memory
     * address, an LMA is load memory address. If VMA
     * and LMA are the same, the syntax is `> VMA`
     *
     * Do you relocate this section during startup code?
     * If it's relocatable VMA and LMA are different.
     */
    } > FLASH

    _la_data = LOADADDR(.data);

    .data :
    {
        /**
         * Symbol declaration, maps the address in the current
         * location (. is the location counter) of the section
         * to a symbol name.
         *
         * What this does is add a new entry to the symbol
         * table in the final executable. The compiler is unaware
         * of this, because this is happening at the linking stage.
         *
         * It's important to know that the location counter always
         * tracks VMA of the section in which it is being used.
         */
        _sdata = .;
        *(.data)
        _edata = .;
    /**
     * This section will be relocated from FLASH to RAM
     * during startup.
     */
    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;
        /* TODO: what is __bss_start__ */
        __bss_start__ = _sbss;
        *(.bss)
        _ebss = .;
        __bss_end__ = _ebss;
    } > RAM

    _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}