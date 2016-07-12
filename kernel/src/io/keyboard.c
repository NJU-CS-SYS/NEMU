#include "x86.h"
#include "common.h"
#include "vga.h"

#define KEYBOARD_PORT_BASE 0x60
#define BUF_HEAD_PORT      0x10
#define BUF_TAIL_PORT      0x40
#define KEYBOARD_IRQ 1

char get_char() {
    uint8_t buf_head = in_byte(BUF_HEAD_PORT);
    uint8_t buf_tail = in_byte(BUF_TAIL_PORT);

    while(buf_head == buf_tail) {
        buf_tail = in_byte(BUF_TAIL_PORT);
        buf_head = in_byte(BUF_HEAD_PORT);
    }


    // get the first character
    char c = in_byte(KEYBOARD_PORT_BASE + buf_head);

    out_byte(VMEM_DATA_PORT_BASE, c);
    return c;
}
