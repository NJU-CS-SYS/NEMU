#include "device/i8259.h"
#include "ui/ui.h"

#define TIMER_IRQ 0
static uint64_t count;
void timer_intr() {
	if(nemu_state == RUNNING) {
		count ++;
		printf("timer! %llu\n", count);
		fflush(stdout);
		i8259_raise_intr(TIMER_IRQ);
	}
}

void init_timer() {
	count = 0;
}
