#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEYUP 0
#define KEYDOWN 1
#define KEYBOARD_IRQ 1

uint8_t get_scancode();
void keyboard_intr(uint8_t);
int key_type(uint32_t);
bool npc_keyboardintr();
char npc_getchar();

#endif