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

#include <stdlib.h>
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
