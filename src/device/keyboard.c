#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"
#include "monitor.h"

#define I8042_DATA_PORT 0x60
#define KEYBOARD_IRQ 1

static uint8_t *i8042_data_port_base;
static bool newkey;

void keyboard_intr(uint8_t scancode) {
    if(nemu_state == RUNNING && newkey == false) {
        i8042_data_port_base[0] = scancode;
        i8259_raise_intr(KEYBOARD_IRQ);
        newkey = true;
    }
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

#ifdef SYS_LAB
#include <stdlib.h>

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

// 键盘事件回调函数 / 中断处理函数
void kb_callback(int unused)
{
    extern Monitor monitor;
    key_buf[key_buf_tail++] = monitor.key_state->data;
    if (key_buf_tail == KEY_BUF_MAX) key_buf_tail = 0;
    monitor.key_state->ready = 0;
}
#endif
