#ifndef __VGA_H__
#define __VGA_H__

#define SCR_WIDTH  1280
#define SCR_HEIGHT 1024
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  ((uint8_t*)0xA0000)

#define WHITE 15

// cursor's position (cur_x, cur_y)
extern int cur_x; 
extern int cur_y;

void draw_character(char ch, int x, int y, int color);
void draw_string(const char *str, int x, int y, int color);
void prepare_vbuffer();
void display_vbuffer();

#endif