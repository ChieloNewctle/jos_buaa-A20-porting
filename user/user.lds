OUTPUT_ARCH(arm)
ENTRY(main)
SECTIONS
{
    . = 0x00400000;

    _text = .;            /* Text and read-only data */
    .text : {
        *(.text.startup)
        *(.text)
        *(.fixup)
        *(.gnu.warning)
    }

    _etext = .;           /* End of text section */

    .data : {         /* Data */
        *(.data)
        *(.rodata)
        *(.rodata.*)
        *(.eh_frame)
        CONSTRUCTORS
    }

    _edata = .;           /* End of data section */


    . = ALIGN(0x1000);
    __bss_start = .;      /* BSS */
    .bss : {
        *(.bss)
    }
    /DISCARD/ : {
        *(.comment)
        *(.debug_*)
    }
    end = . ;

}
