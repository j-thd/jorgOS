/* Self-made linker script for education purposes for the TM4C123GH6PM */

__STACK_SIZE__ = 0x200;
__HEAP_SIZE__ = 0x400;

/*
    I have learned that the standard library implementatoin generate their own custom sections as well, and I'd like
    to place them in well-defined manner, since I want all memory usage and special sections to be very well-defined
    in order to program super-defensively later. Since the GNU linker script language does not seem to support
    placing sections just based on attributes (i.e.: placing all unplaced RO sections in .text or something similar),
     the sections must be placed manually, until everything is cleaned up. This does also mean that if I later switch
      to another STD implementation that I must check again if all sections are placed, and perhaps create a custom
      linker script again for the specific STD implementation.

      Actually, I am not going to place them. I am happy with how the linker is placing the sections currently. The
      important places are still well-defined.
 */


MEMORY /* Section 2.4 (page 92) of the data sheet describes the memory map of the device.  */
{
    /* Info from: https://ftp.gnu.org/old-gnu/Manuals/ld-2.9.1/html_mono/ld.html#SEC16 */

    /* Flash memory is non-volatile memory, i.e.: it retains the data after power is removed.
    After it is programmed it functions as ROM (Read-Only Memory).

    The code can therefore be stored in flash, and so can initialized data. The initialized data is copied to SRAM
    by start-up code. That way it is available for read-write-access.
    */
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000

    /* SRAM (Static Random-Access Memory) holds its data permanently when powered.
       It loses all data when powered off. Obviously, no code and initialized data should go here.
       I.e.: it is volatile memory.

       SRAM is an arbitrary name, so it makes sense to name if after the type of memory.

       The attribute is (rw) because this data can be modified when the chip is running.
       I suppose one could also put code in here, but it would have to be copied in during start-up.
       And it could be modified later.
     */
    SRAM (rw) : ORIGIN = 0x2000000, LENGTH = 0x00008000

}

SECTIONS /* In the SECTIONS part, we must describe what section actually goes where.*/
{
    /* The text section contains code (.text), read-only data (.rodata) and the vector table, which must be placed at
    the start at position 0 of FLASH. The Vector Table was labeled RESET with the AREA command in startup_TM4C123.s

    For armlink did meant referring to RESET in the scatter.txt. I haven't found documentation to imply that the GNU
    linker works the same.
     */
    .text :
    {
        *(.vector_table)
        *(.text)
        *(.text.*) /* The STD used, makes lots of extra sub-sections). Let's put them in the right place */
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4); /* To make sure everything is aligned at 4-byte boundaries/words */
    }> FLASH

    /* The data section contains data that should be initialized at the program start. The actual data is contained
    in FLASH first, and should be copied into SRAM at the program start. Therefore the linker needs to find a place
    in FLASH, but also in SRAM to contain the data. Startup code should copy the data

     */
    .data :
    {
        /* Store the location of the start of the data section so the startup code can use it to copy the data to
        SRAM */
        __start_of_data = .; /* The dot "." operator tracks VMA (where the section resides at program start */
        /* This defines a symbol that tells us where data is placed in the load region. */
        __load_address_data = LOADADDR(.data);
        *(.data)
        *(.data.*)
        . = ALIGN(4); /* To make sure everything is aligned at 4-byte boundaries/words */
        __end_of_data = .; /* The end also needs to be known, of course. */
    }> SRAM AT> FLASH

    /* The bss section contains that should be zero when initialized. It can be located in SRAM, but needs to be set
    to zero by the startup code.
     */
    .bss :
    {
        __bss_start__ = .; /* The __bss_start__ and __bss_end__ symbol are expected by the _start initialize it to 0 */
        *(.bss)
        *(.bss.*)
        . = ALIGN(4); /* To make sure everything is aligned at 4-byte boundaries/words */
        __bss_end__ = .;

    }> SRAM

    /* The stack grows down, so the end goes at the start of the section. */
    .stack :
    {

        . = ALIGN(4);
        __stack_end__ = .;
        . = . + __STACK_SIZE__;
        . = ALIGN(4);
        __stack_start__ = .;
    }> SRAM

    .heap :
    {
        . = ALIGN(4);
        __heap_start__ = .;
        end = __heap_start__; /* _sbrk needs to this symbol to denote the heap end. Usually it goes at the of .bss.
        Not sure which size the heaps grows yet. Not using the heap yet, and I'd probably want a custom implementation anyway.
        Basically, the stack and heap flowing out of their boundaries should be illegal and trip an error. This will
        force me to learn how to size them appropriatelly and understand the space code takes, whilst also preventing
         silent, deadly errors.*/
         . = . + __HEAP_SIZE__;
         . = ALIGN(4);
        __heap_end__ = .;
    }> SRAM


    /* the _sbrk function of the nanolib-library (or perhaps in general) expects the heap after the .bss section, and
     it is denoted with the "end" symbol.


     I'm not sure how it is done, but I think it would be nice if I could denote how big the stack and the heap are
     and reserving the entire places. It might be as simple and calling a new section .heap and placing it after
     .bss? I could look into this later after I get things to work the normal way. I'm making a huge detour already.

     */
}