#ifndef __VGA_H__
#define __VGA_H__

#define SCR_WIDTH  1280
#define SCR_HEIGHT 1024
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  (0xE0000000)

#define WHITE 15

extern int cur_x;
extern int cur_y;

void draw_string(const char *, int, int, int);
void display_buffer();
void prepare_buffer();

#endif