#include "common.h"
#include "vga.h"

int int2str(int num, char *buf, int buf_size);

void write_int(int num) {
	char buffer[20];

	if( int2str(num, buffer, 20) ) {
		draw_string(buffer);
	}
}

int int2str(int num, char *buf, int buf_size) {
	if(buf == NULL) return -1;

    char buf_t[100];
	int i = 0, len = 0, digit;
	if( num < 0 ) {
		buf_t[i ++] = '-';
		num = -num;
	}

	do {
		digit = num % 10;
		num = num / 10;
		buf_t[i ++] = (char)(digit + '0');
	} while(num && i < buf_size - 1);
    buf[i] = '\0';
    len = i;

    int j = 0;
    if (buf_t[0] == '-') {
        buf[0] = '-';
        j = 1;
    }
    for (; j < len; j ++) {
        buf[j] = buf_t[--i];
    }

    return len;
}
