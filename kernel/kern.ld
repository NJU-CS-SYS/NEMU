ENTRY(start)
SECTIONS
{
    . = 0xc0100000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
