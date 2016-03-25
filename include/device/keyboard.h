#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KCODE_TYPE uint32_t

#define NOT_SCANCODE 0
#define IS_SCANCODE 1

static inline int is_scancode(KCODE_TYPE code) {
	uint8_t (*pcode)[4] = &code;
	if( pcode[1] == 0xf0 )
		return NOT_SCANCODE;
	else if ( pcode[1] == 0x00)
		return IS_SCANCODE;

	return NOT_SCANCODE;
}

char kcode2char(KCODE_TYPE code);
char get_char();

#endif