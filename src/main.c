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

    main_loop();

    return 0;
}
