#ifndef __VGA_H__
#define __VGA_H__

#include "common.h"

#define SCREEN_ROW 1024
#define SCREEN_COL 1280
#define VGA_HZ 25

extern uint8_t (*pixel_buf) [SCREEN_COL];

static inline void draw_pixel(int x, int y, uint8_t color_idx) {
    assert(x >= 0 && x < SCREEN_COL && y >= 0 && y < SCREEN_ROW);
    pixel_buf[y][x] = color_idx;
}

#endif
