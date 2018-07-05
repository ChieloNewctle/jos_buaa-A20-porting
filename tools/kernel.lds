OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
    . = 0x80000000;
    .text.except_vec : { *(.text.except_vec) }

    . = 0x80010000;
    .text.boot : { *(.text.boot) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss : { *(.bss) }

    __end = .;
}
