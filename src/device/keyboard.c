#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"
#include "keyboard.h"
#include "monitor.h"

#define I8042_DATA_PORT 0x60
#define I8042_BUF_HEAD_PORT 0x76
#define I8042_BUF_TAIL_PORT 0x77

static uint8_t *i8042_data_port_base;
static uint8_t *key_buf_head_port;
static uint8_t *key_buf_tail_port;

static bool newkey;

void keyboard_intr(uint8_t c) {
    if(nemu_state == RUNNING && newkey == false) {
        i8042_data_port_base[0] = c;
        i8259_raise_intr(KEYBOARD_IRQ);
        newkey = true;
    }
}

uint8_t char2keycode(char c);
char kcode2char(uint8_t code);

#ifdef SYS_LAB
#include <stdlib.h>

#define KEY_BUF_MAX 128
static char key_buf[KEY_BUF_MAX];
static int key_buf_head = 0;  // inclusive
static int key_buf_tail = 0;  // exclusive

char buf_read() {
    char c = key_buf[key_buf_head ++];
    if (key_buf_head == KEY_BUF_MAX) key_buf_head = 0; 
    key_buf_head_port[0] = key_buf_head;
    return c;
}

void buf_write(char c) {
    key_buf[key_buf_tail ++] = c;
    if (key_buf_tail == KEY_BUF_MAX) key_buf_tail = 0; 
    key_buf_tail_port[0] = key_buf_tail;
}

void i8042_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    if(!is_write) {
        newkey = false;   
        // remove the read char in npc buffer, it is already read through PORT
        buf_read();
    }
}

void buf_head_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    if(is_write) {
        key_buf_head = key_buf_head_port[0];
    }
}

void buf_tail_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    if(is_write) {
        key_buf_tail = key_buf_tail_port[0];
    }
}
void init_i8042() {
    i8042_data_port_base = add_pio_map(I8042_DATA_PORT, KEY_BUF_MAX, i8042_io_handler);
    key_buf_head_port    = add_pio_map(I8042_BUF_HEAD_PORT, 1, buf_head_io_handler);
    key_buf_tail_port    = add_pio_map(I8042_BUF_TAIL_PORT, 1, buf_tail_io_handler);
    newkey = false;
}

char npc_getc()
{
    // Read a char as soon as the input buffer is available.
    while (key_buf_head == key_buf_tail) {}
    char ch = buf_read();
    
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
        
        case 0x1C: return 'a';
        case 0x32: return 'b';
        case 0x21: return 'c';
        case 0x23: return 'd';
        case 0x24: return 'e';
        case 0x2B: return 'f';
        case 0x34: return 'g';
        case 0x33: return 'h';
        case 0x43: return 'i';
        case 0x3B: return 'j';
        case 0x42: return 'k';
        case 0x4B: return 'l';
        case 0x3A: return 'm';
        case 0x31: return 'n';
        case 0x44: return 'o';
        case 0x4D: return 'p';
        case 0x15: return 'q';
        case 0x2D: return 'r';
        case 0x1B: return 's';
        case 0x2C: return 't';
        case 0x3C: return 'u';
        case 0x2A: return 'v';
        case 0x1D: return 'w';
        case 0x22: return 'x';
        case 0x35: return 'y';
        case 0x1A: return 'z';
        case 0x5A: return '\n';
        default: return 0;
    }
}

uint8_t char2keycode(char c) {
    switch(c) {
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
        case 'a': return 0x1C;
        case 'b': return 0x32;        
        case 'c': return 0x21;
        case 'd': return 0x23;
        case 'e': return 0x24;
        case 'f': return 0x2B;
        case 'g': return 0x34;
        case 'h': return 0x33;
        case 'i': return 0x43;
        case 'j': return 0x3B;
        case 'k': return 0x42;
        case 'l': return 0x4B;
        case 'm': return 0x3A;
        case 'n': return 0x31;
        case 'o': return 0x44;
        case 'p': return 0x4D;
        case 'q': return 0x15;
        case 'r': return 0x2D;
        case 's': return 0x1B;
        case 't': return 0x2C;
        case 'u': return 0x3C;
        case 'v': return 0x2A;
        case 'w': return 0x1D;
        case 'x': return 0x22;
        case 'y': return 0x35;
        case 'z': return 0x1A;
        case '\n': return 0x5A;
        default  : return 0;
    }
}

// 键盘事件回调函数 / 中断处理函数
void kb_callback(int unused)
{
    extern Monitor monitor;
    
    uint8_t data = monitor.key_state->data;
    uint8_t scancode = char2keycode(data);
    char c  = kcode2char(scancode);

    buf_write(c);

    monitor.key_state->ready = 0;
}
#endif
