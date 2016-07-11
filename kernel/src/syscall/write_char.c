#include "common.h"
#include "vga.h"

void write_char(unsigned c) {
	char buffer[2];
	char c_byte = (char)c;
	buffer[0] = c_byte;
	buffer[1] = 0;

	draw_string(buffer);
}
