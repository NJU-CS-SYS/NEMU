#include "device/i8259.h"
#include "ui/ui.h"

#include "cpu/reg.h"
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
