ENTRY(_start)

SECTIONS {
    . = 0x80000;

    /* Defining where a specific section goes. */

    .text.boot : {
        *(.text.boot)
    }

    .text : {
        *(.text)
    }

    /* Read-only data */
    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    . = ALIGN(8); /* Aligns the location counter to 8 bytes */

    __bss_start = .;

    .bss : {
        *(.bss)
    }

    __bss_end = .;
}

