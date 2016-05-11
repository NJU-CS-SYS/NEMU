#include "common.h"
#include "x86.h"
#include <string.h>
#include "vga.h"

extern char font8x8_basic[128][8];

void draw_character(char ch) {
    assert((ch & 0x80) == 0); // is not a ASCII character
    
    // to monitor
    out_byte(VMEM_DATA_PORT_BASE, ch);
}

void draw_string(const char *str) {
    while (*str) {
        draw_character(*str ++);
    }
}