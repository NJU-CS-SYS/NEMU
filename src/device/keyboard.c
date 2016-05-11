#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"
#include "keyboard.h"
#include "monitor.h"

#define I8042_DATA_PORT 0x60

static uint8_t *i8042_data_port_base;
static bool newkey;

void keyboard_intr(uint8_t c) {
    if(nemu_state == RUNNING && newkey == false) {
        i8042_data_port_base[0] = c;
        i8259_raise_intr(KEYBOARD_IRQ);
        newkey = true;
    }
}

uint8_t char2keycode(char c);

void i8042_io_handler(ioaddr_t addr, size_t len, bool is_write) {
    if(!is_write) {
        newkey = false;
    }
}

void init_i8042() {
    i8042_data_port_base = add_pio_map(I8042_DATA_PORT, 1, i8042_io_handler);
    newkey = false;
}

#ifdef SYS_LAB
#include <stdlib.h>

char npc_getchar() {
    extern volatile Monitor monitor;
    char c = monitor.key_state->data;
    uint8_t scancode = char2keycode(ch);
    char ch  = kcode2char(scancode); 
    monitor.key_state->ready = 0;
    return ch;
}

#define KEY_BUF_MAX 128
static char key_buf[KEY_BUF_MAX];
static int key_buf_head = 0;  // inclusive
static int key_buf_tail = 0;  // exclusive

char npc_getc()
{
    // Read a char as soon as the input buffer is available.
    while (key_buf_head == key_buf_tail) {}
    char ch = key_buf[key_buf_head++];
    if (key_buf_head == KEY_BUF_MAX) key_buf_head = 0;
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

// 键盘事件回调函数 / 中断处理函数
void kb_callback(int unused)
{
    extern Monitor monitor;
    key_buf[key_buf_tail++] = monitor.key_state->data;
    if (key_buf_tail == KEY_BUF_MAX) key_buf_tail = 0;
    monitor.key_state->ready = 0;
}
#endif
