#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEYUP 0
#define KEYDOWN 1
#define KEYBOARD_IRQ 1

void keyboard_intr(uint8_t);
char npc_getc();
void npc_gets(char buf[], size_t size);

#endif