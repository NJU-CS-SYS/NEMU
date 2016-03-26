#include "x86.h"
#include "common.h"
#include "vga.h"
#include <string.h>

#define KEYBOARD_PORT 0x60
#define KEYBOARD_IRQ 1

#define STREAM_SIZE 128

static char stdin_stream[STREAM_SIZE];
static int next_index = 0; // the last byte's index plus one

void keyboard_event(void);
void handle_key(uint8_t);
char kcode2char(uint8_t);
extern void add_irq_handle(int, void(*)(void));

void init_kb() {
	memset(stdin_stream, 0, STREAM_SIZE);
	add_irq_handle(KEYBOARD_IRQ, keyboard_event);
}

static inline int is_keydown(uint8_t code) {
	return ( (code & 0x80) == 0x80 ) ? 0 : 1; 
}

void keyboard_event() {
    int key_code = in_byte(KEYBOARD_PORT);
    if( is_keydown(key_code) )
    	handle_key(key_code);
}

void handle_key(uint8_t code) {
    if( next_index < STREAM_SIZE ) {
      	char c = kcode2char(code);
      	
        if( c != 0 ) {
      		char cb[2] = {c, '\0'};
        	stdin_stream[next_index ++] = c;
        	draw_string(cb, cur_x, cur_y + 8, 15); // WHITE
        	display_buffer(); // display the pressing key
        }
    }
}

char get_char() {
    char c;
    int i;

    // no character in the stream
    if( next_index < 0 ) 
        return 0;

    // get the first character
    c = stdin_stream[0];
    // remove the first character
    for( i = 0; i < next_index - 1; i ++) {
        stdin_stream[i] = stdin_stream[i + 1]; 
    }
    next_index --;
    return c;
}

char kcode2char(uint8_t code) {
    switch(code) {
        case 0x70:
        case 0x45: return '0';

        case 0x69:
        case 0x16: return '1';
        
        case 0x72:
        case 0x1E: return '2';
        
        case 0x7A:
        case 0x26: return '3';
        
        case 0x6B:
        case 0x25: return '4';
        
        case 0x73:
        case 0x2E: return '5';
        
        case 0x74:
        case 0x36: return '6';
        
        case 0x6C:
        case 0x3D: return '7';
        
        case 0x75:
        case 0x3E: return '8';
        
        case 0x7D:
        case 0x46: return '9';
        
        case 0x1C: return 'A';
        case 0x32: return 'B';
        case 0x21: return 'C';
        case 0x23: return 'D';
        case 0x24: return 'E';
        case 0x2B: return 'F';
        case 0x34: return 'G';
        case 0x33: return 'H';
        case 0x43: return 'I';
        case 0x3B: return 'J';
        case 0x42: return 'K';
        case 0x4B: return 'L';
        case 0x3A: return 'M';
        case 0x31: return 'N';
        case 0x44: return 'O';
        case 0x4D: return 'P';
        case 0x15: return 'Q';
        case 0x2D: return 'R';
        case 0x1B: return 'S';
        case 0x2C: return 'T';
        case 0x3C: return 'U';
        case 0x2A: return 'V';
        case 0x1D: return 'W';
        case 0x22: return 'X';
        case 0x35: return 'Y';
        case 0x1A: return 'Z';
        case 0x5A: return '\n';
        default: return 0;
    }
}