#include "x86.h"
#include "common.h"
#include "vga.h"

#define KEYBOARD_PORT_BASE 0x60
#define KEYBOARD_IRQ 1

char get_char() {
    // get the first character
    char c = in_byte(KEYBOARD_PORT_BASE);

    out_byte(VMEM_DATA_PORT_BASE, c);
    return c;
}
