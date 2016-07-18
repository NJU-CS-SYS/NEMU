#include "common.h"

void main_loop();
void init_device();

int enable_debug = false;
int quiet = false;

int main(int argc, char *argv[]) {
    enable_debug = true;
    quiet = false;

    /* Initialization of device */
    init_device();

    extern uint8_t pio_space[];
    Log("io port ide %02x at %08x", pio_space[0x1f7], (unsigned int)(pio_space + 0x1f7));
    main_loop();

    return 0;
}
