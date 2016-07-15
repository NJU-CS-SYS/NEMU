#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"
#include "keyboard.h"

#define I8042_DATA_PORT 0x60
#define I8042_BUF_HEAD_PORT 0x10
#define I8042_BUF_TAIL_PORT 0x40
#define KEY_BUF_MAX 128

static uint8_t *i8042_data_port_base;
static bool newkey;

// not used
void keyboard_intr(uint8_t c) {
    if(nemu_state == RUNNING && newkey == false) {
        i8042_data_port_base[0] = c;
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
