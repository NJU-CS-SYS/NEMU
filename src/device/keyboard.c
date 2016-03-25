#include "io/port-io.h"
#include "device/i8259.h"
#include "ui/ui.h"

#define I8042_DATA_PORT 0x60
#define KEYBOARD_IRQ 1

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
    return *( (uint32_t *)KMEM_ADDR );
}

uint8_t scancode2byte(uint32_t code) {
    char (*p)[4] = (char *)(&code);
    return code[0];
}

int key_type(uint32_t code) {
    char (*p)[4] = (char *)(&code);
    if(code[1] == 0xf0)
        return KEYUP;
    else if(code[1] == 0x00)
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

