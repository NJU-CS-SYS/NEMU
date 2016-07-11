ENTRY(start)
SECTIONS
{
    . = 0xc0200000;
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
