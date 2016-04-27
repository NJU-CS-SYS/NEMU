#include "common.h"
#include "vga.h"

void write_int(unsigned c) {
	char buffer[2];
	char c_byte = (char)c;
	buffer[0] = c_byte;
	buffer[1] = 0;

	draw_string(buffer, cur_x, cur_y + 8, WHITE);
		
	Log("in write_char, before display_buffer");
	display_buffer();
	cur_x += 8; // '\n'
}