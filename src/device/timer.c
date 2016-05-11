#include "common.h"
#include "device/i8259.h"
#include "ui/ui.h"

#define TIMER_IRQ 0

static uint64_t count;

void timer_intr() {
    if(nemu_state == RUNNING) {
        i8259_raise_intr(TIMER_IRQ);
        count ++;
    }
}

void init_timer() {
    count = 0;
}

static int rt_cnt = 0;  // real timer counter
void timer_callback(int unused)
{
    printf("timer event %d\n", rt_cnt++);
}
