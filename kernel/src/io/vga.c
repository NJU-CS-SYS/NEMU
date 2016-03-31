#include "common.h"
#include "x86.h"
#include <string.h>
#include "vga.h"

extern char font8x8_basic[128][8];

static uint8_t vbuf[SCR_SIZE];
uint8_t *vmem = vbuf;
int cur_x = 0;
int cur_y = 0;

static inline void draw_pixel(int x, int y, int color) {
    assert(x >= 0 && y >= 0 && x < SCR_HEIGHT && SCR_WIDTH);
    vmem[(x << 8) + (x << 6) + y] = color;
}

void draw_character(char ch, int x, int y, int color) {
    assert((ch & 0x80) == 0); // is not a ASCII character

    int i, j;
    char *p = font8x8_basic[(int)ch];
    for (i = 0; i < 8; i ++) 
        for (j = 0; j < 8; j ++) 
            if ((p[i] >> j) & 1)
                draw_pixel(x + i, y + j, color);

    cur_x = x;
    cur_y = y;
}

void draw_string(const char *str, int x, int y, int color) {
    while (*str) {
        draw_character(*str ++, x, y, color);
        if (y + 8 >= SCR_WIDTH) {
            x += 8; 
            y = 0;
        } 
        else {
            y += 8;
        }
    }
}

void prepare_buffer() {
	memset(vmem, 0, SCR_SIZE);
    cur_x = cur_y = 0;
}

void display_buffer() {
    Log("in display_buffer");
    asm volatile ("cld; rep movsl" : : "c"(SCR_SIZE / 8), "S"(vmem), "D"(VMEM_ADDR));
}