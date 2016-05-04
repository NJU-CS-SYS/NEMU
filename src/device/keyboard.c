#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"
#include "keyboard.h"
#include "monitor.h"

#define I8042_DATA_PORT 0x60

#define KMEM_ADDR 0x9999

static uint8_t *i8042_data_port_base;
static bool newkey;

void keyboard_intr(uint8_t scancode) {
    if(nemu_state == RUNNING && newkey == false) {
        i8042_data_port_base[0] = scancode;
        i8259_raise_intr(KEYBOARD_IRQ);
        newkey = true;
    }
}

uint32_t get_scancode() {
    //uint32_t *p_kmem = (uint32_t *)KMEM_ADDR;
    //return *p_kmem;
    char c = npc_getchar();
    return char2keycode(c);
}

uint8_t scancode2byte(uint32_t code) {
    uint8_t *p = (uint8_t*)(&code);
    return p[0];
}

int key_type(uint32_t code) {
    uint8_t *p = (uint8_t *)(&code);
    if(p[1] == 0xf0)
        return KEYUP;
    else if(p[1] == 0x00)
        return KEYDOWN;

    return KEYUP;
}

void i8042_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    if(!is_write) {
        newkey = false;
    }
}

void init_i8042() {
    i8042_data_port_base = add_pio_map(I8042_DATA_PORT, 1, i8042_io_handler);
    newkey = false;
}

#include <stdlib.h>
bool npc_keyboardintr() {
    extern volatile Monitor monitor;
    return monitor.key_state->ready;
}

char npc_getchar() {
    extern volatile Monitor monitor;
    char ch = monitor.key_state->data;
    monitor.key_state->ready = 0;
    return ch;
}

char npc_getc()
{
    extern volatile Monitor monitor;
    while (!monitor.key_state->ready) ;
    char ch = monitor.key_state->data;
    monitor.key_state->ready = 0;
    return ch;
}

void npc_gets(char buf[], size_t size)
{
    char ch;
    char *end = buf + size - 1;
    while ((ch = npc_getc()) != '\n' && buf < end) {
        *buf++ = ch;
    }
    *buf = '\0';
}

uint32_t char2keycode(char c) {
    switch(code) {
        case '0': return 0x45;
        case '1': return 0x16; 
        case '2': return 0x1E;
        case '3': return 0x26;
        case '4': return 0x25;
        case '5': return 0x2E;
        case '6': return 0x36;
        case '7': return 0x3D;
        case '8': return 0x3E;
        case '9': return 0x46;
        case 'A': return 0x1C;
        case 'B': return 0x32;        
        case 'C': return 0x21;
        case 'D': return 0x23;
        case 'E': return 0x24;
        case 'F': return 0x2B;
        case 'G': return 0x34;
        case 'H': return 0x33;
        case 'I': return 0x43;
        case 'J': return 0x3B;
        case 'K': return 0x42;
        case 'L': return 0x4B;
        case 'M': return 0x3A;
        case 'N': return 0x31;
        case 'O': return 0x44;
        case 'P': return 0x4D;
        case 'Q': return 0x15;
        case 'R': return 0x2D;
        case 'S': return 0x1B;
        case 'T': return 0x2C;
        case 'U': return 0x3C;
        case 'V': return 0x2A;
        case 'W': return 0x1D;
        case 'X': return 0x22;
        case 'Y': return 0x35;
        case 'Z': return 0x1A;
        case '\n': return 0x5A;
        default  : return 0;
    }
}
