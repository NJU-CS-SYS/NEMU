#ifndef __VGA_H__
#define __VGA_H__

#define WHITE 15

extern int cur_x;
extern int cur_y;

extern void draw_string(char *, int, int, int);
extern void display_buffer();
extern void prepare_buffer();

#endif