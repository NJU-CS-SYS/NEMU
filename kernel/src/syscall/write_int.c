#include "common.h"
#include "vga.h"

int int2str(int num, char *buf, int buf_size);

void write_int(int num) {
	char buffer[20];

	if( int2str(num, buffer, 20) ) {
		draw_string(buffer, cur_x, cur_y + 8, WHITE);
		display_buffer();
		cur_x += 8; // '\n'
	}
}

int int2str(int num, char *buf, int buf_size) {
	if(buf == NULL) return -1;

	int i = 0, digit;
	if( num < 0 ) {
		buf[i ++] = '-';
		num = -num;
	}

	do {
		digit = num % 10;
		num = num / 10;
		buf[i ++] = (char)(digit + '0');
	} while(num && i < buf_size - 1);
    buf[i] = '\0';

    return i;
}