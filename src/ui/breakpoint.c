#include "ui/breakpoint.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].addr = 0;
		bp_pool[i].value = 0;
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO = NR_BP - 1;
	bp_pool[i].next = NULL;

	head = NULL;
	free_ = bp_pool;
}

/* TODO: Implement the function of breakpoint */

BP* new_bp() {
	/* modify free like a stack !? */
	if (free_ == NULL) assert(0);
	BP* temp = free_;
	free_ = free_->next;
	return temp;
}

void free_bp(BP *bp) {
	bp->next = free_;
	free_ = bp;
}

void add_bp(swaddr_t addr) {
	/* add new breakpoint to the head */
	BP* temp = new_bp();
	temp->next = head;
	head = temp;
	temp->addr = addr;
	temp->value = swaddr_read(addr, 1);
}
