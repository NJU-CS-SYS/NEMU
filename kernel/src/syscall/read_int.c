#include "common.h"
#include "vga.h"
#include "keyboard.h"

extern char get_char();
extern void draw_string();
extern display_buffer();

int str2int(char *b);

int read_int(char *prompt) {
	// display prompt	
	draw_string(prompt, cur_x, cur_y + 8, WHITE);
	display_buffer();

	// read
	char buffer[20];
	int i = 0;
	char c;
	while( (c = get_char()) != '\n' ) {
		buffer[i ++] = c;
	}
	buffer[i] = '\0';
	return str2int(buffer);
}

int str2int(char *str) {
	int n = 0;
	while( *str ) { 
		n = n * 10 + ( (*str) - '0' );
		str ++;
	}
	return n;
}
