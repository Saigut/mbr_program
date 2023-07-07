ENTRY(main_func)
  
SECTIONS
{
    . = 0x7C00;    /* Loading address */

    .text : AT(0x7C00)    /* Position of the text section */
    {
        *(.text)
    }

    .sig : AT(0x7DFE)    /* Position of the boot sector signature */
    {
        SHORT(0xAA55)
    }

    /DISCARD/ :
    {
        *(.eh_frame)
        *(.comment)
    }

    . = 0x7E00;  /* Ensuring that we have 512 bytes of space */
}
