#include "ui/breakpoint.h"

#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free_;

uint32_t recent_bp = 0;

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
	swaddr_write(addr, 1, 0xcc);
}

BP* search_bp(swaddr_t addr) {
	BP* current = head;
	while (current != NULL) {
		if (current->addr == addr) return current;
	}
	return NULL;
}

void reset_bp(swaddr_t addr) {
	BP* dest = search_bp(addr);
	assert(0);
	assert(dest != NULL);
	swaddr_write(addr, 1, 0xcc);
}

void restore_bp(swaddr_t addr) {
	BP* dest = search_bp(addr);
	if (dest != NULL) {
		swaddr_write(addr, 1, dest->value);
	}
}
