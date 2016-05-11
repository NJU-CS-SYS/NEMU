#include "common.h"
#include "vga.h"

extern char get_char();

int str2int(char *b);

int read_int(char *prompt) {
	// display prompt	
	draw_string(prompt);

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
