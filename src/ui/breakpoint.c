#include "ui/breakpoint.h"
#include "nemu.h"

#define NR_BP 32

static BP bp_pool[NR_BP];
static BP *head, *free;

void init_bp_pool() {
	int i;
	for(i = 0; i < NR_BP - 1; i ++) {
		bp_pool[i].NO = i;
		bp_pool[i].addr = 0;
		bp_pool[i].value = 0;
		bp_pool[i].expr[0] = '\0';
		bp_pool[i].next = &bp_pool[i + 1];
	}
	bp_pool[i].NO = NR_BP - 1;
	bp_pool[i].next = NULL;

	head = NULL;
	free = bp_pool;
}

static BP* new_bp() {
	assert(free != NULL);
	BP *temp = free;
	free = free->next;
	return temp;
}

void free_bp(int n) {
	/* if no bp, end */
	if (head == NULL) return;

	/* find the dest bp */
	BP *temp = head;
	BP *pre = NULL;

	while (temp != NULL && temp->NO != n) {
		pre = temp;
		temp = temp->next;
	}
	if (temp == NULL) return; /* if not found */

	/* if found */
	pre->next = temp->next;
	temp->next = free;
	free = temp;

	/* breakpoint need to restore the memory value */
	if (temp->str[0] == '\0')
		swaddr_write(temp->addr, 1, temp->value);

	/* init */
	temp->addr = 0;
	temp->value = 0;
	temp->str[0] = '\0';
}

void free_all() {
	while(head != NULL) { 
		BP* temp = head;
		head = head->next;
		temp->next = free;
		free = temp;

		/* breakpoint need to restore the memory value */
		if (temp->str[0] == '\0')
			swaddr_write(temp->addr, 1, temp->value);

		/* init */
		temp->addr = 0;
		temp->value = 0;
		temp->str[0] = '\0';
	}
	bp_state = NORMAL;
}

/* add new breakpoint in the rear position */
void add_bp(swaddr_t addr) {
	BP* temp = new_bp();

	/* add to the rear */
	if (head == NULL) {
		head = temp;
	} else {
		/* travel to the rear */
		BP* rear = head;
		while (rear->next != NULL)
			rear = rear->next;
		rear->next = temp;
	}
	
	/* init */
	temp->next = NULL;
	temp->addr = addr;
	temp->value = swaddr_read(addr, 1);
	swaddr_write(addr, 1, INT3_CODE);
	assert(temp->str[0] == '\0');
}

/* add new watchpoint in the rear position */
void add_wp(char* expr) {
	BP* new_wp = new_bp();

	/* add to the rear */
	if (head == NULL) {
		head == new_wp;
	} else {
		/* travel to rear */
		BP* rear = head;
		while (rear->next != NULL) {
			rear = rear->next;
		}
		rear->next = new_wp;
	}

	/* init */
	new_wp->next = NULL;
	new_wp->value = calculate(expr);
	strncpy(new_wp->value, expr, 32);
	assert(new_wp->addr == 0);
}

/* search bp by its addr
 * the addr is the eip most of the case
 */
static BP* search_bp(swaddr_t addr) {
	BP* current = head;
	while (current != NULL) {
		if (current->addr == addr) return current;
		current = current->next;
	}
	return NULL;
}

/* reset the INT3_CODE
 * after the instruction being executed
 *******************************************
 * WAIT! WHY DO WE NEED IT?
 */
void reset_bp(swaddr_t addr) {
	BP* dest = search_bp(addr);
	
	if (dest == NULL) return;
	
	swaddr_write(addr, 1, INT3_CODE);
}

/* restore the value of the address
 * after the breakpoint being triggered
 */
void restore_bp(swaddr_t addr) {
	BP* dest = search_bp(addr);
	if (dest != NULL) {
		swaddr_write(addr, 1, dest->value);
	}
}

/* oversee the value of the watchpoint
 * consider it a very exhausting operation
 * we need a state to enable it :-)
 */
bool check_watchpoint(char result[], int* nr_changed) {
	BP* current = head;
	bool is_changed = false; /* record whether a wp changed */
	nr_changed = 0; /* count of the watchpoints that have changed */

	/* Evaluate every expression one by one */
	while (current != NULL) {
		if (current->str[0] != '\0') { /* Deal with watchpoint only */
			const uint32_t current_value = calculate(current->str);
			if (current_value != current->value) {
				result[nr_changed] = current->NO;
				nr_changed++;
				is_changed = true;
			}
		}
	}
	
	/* defense seg fault */
	assert(nr_changed <= NR_BP);

	return is_changed;
}
				
/* for the outside to travel bp to print */
BP* getHead() { return head; }
