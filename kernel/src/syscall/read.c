#include "common.h"
#include "vga.h"

extern char get_char();

int str2int(char *b, int i);

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
	return str2int(buffer, --i);
}

int str2int(char *str, int m) {
    int n = 0;
    //for (int i = m; i >= 0; i --) {
    for (int i = 0; i <= m; i ++) {
		n = n * 10 + ( (str[i]) - '0' );
	}
	return n;
}

char read_char() {
	char c = get_char();
	return c;
}
