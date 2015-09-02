#include "trap.h"
void loader();

void init() {
    /* Perform remaining initialization */

    /* Load the program, and jump to it */
    loader();

    /* Should never reach here */
    nemu_assert(0);
}
