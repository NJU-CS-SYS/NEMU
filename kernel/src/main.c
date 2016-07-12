#include "common.h"
#include "memory.h"
#include "x86/cpu.h"
#include "trap.h"

void init_ide();
//void init_kb();
void init_i8259();
void init_idt();
void init_mm();
uint32_t loader();

extern uint32_t brk;

void fun_color(void);
/* Initialization phase 2 */
void init_cond() {
    //brk = 0;
#ifdef IA32_INTR
    /* Reset the GDT, since the old GDT in start.S cannot be used in the future. */
    //init_segment();

    /* Set the IDT by setting up interrupt and exception handlers.
     * Note that system call is the only exception implemented in NEMU.
     */
    init_idt();
#endif

#ifdef HAS_DEVICE
    /* Initialize the intel 8259 PIC (Programmable interrupt controller). */
    init_i8259();

    /* Initialize the serial port. After that, you can use printk() to output messages. */
    //init_serial();

    /* Initialize the IDE driver. */
    //Log("init ide...");
    init_ide();

    /* Initialize the keyboard. */
    //init_kb();

#endif
    /* Output a welcome message. Note that the output is actually
     * performed only when the serial port is available in NEMU.
     */

    /* Load the program. */
    uint32_t eip = loader();

    /* Here we go! */
    ((void(*)(void))eip)();

    HIT_GOOD_TRAP;
}

/* Initialization phase 1
 * The assembly code in start.S will finally jump here.
 */
void init() {
    /* Jump to init_cond() to continue initialization. */
    asm volatile("jmp *%0" : : "r"(init_cond));

    /* Should never reach here. */
    nemu_assert(0);
}

//void fun_color() /* Draw some */
/*{
    char *color = (char *)0xa0000;
    int color_idx;
    for (color_idx = 0; color_idx < 320 * 200; color_idx ++)
        color[color_idx] = 0x0f;
}*/
