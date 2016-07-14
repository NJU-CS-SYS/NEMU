#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#define KEYBOARD_IRQ 1

#define I8042_DATA_PORT 0x60

void keyboard_intr(uint8_t);
char npc_getc();
void npc_gets(char buf[], size_t size);
char kcode2char(uint8_t);

#endif
