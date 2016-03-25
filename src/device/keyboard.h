#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEYUP 0
#define KEYDOWN 1

uint32_t get_scancode();
uint8_t scancode2byte(uint32_t);
void keyboard_intr(uint8_t);
int key_type(code);

#endif