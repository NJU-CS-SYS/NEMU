#ifndef __VGA_H__
#define __VGA_H__

#include "common.h"

#define VMEM_ADDR (0xA0000)
#define SCREEN_ROW 1024
#define SCREEN_COL 1280
#define SCR_SIZE ((SCREEN_ROW) * (SCREEN_COL))
#define VGA_HZ 25

extern uint8_t *pixel_buf;

#endif
