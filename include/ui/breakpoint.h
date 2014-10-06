#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc

typedef struct breakpoint {
	int NO;
	swaddr_t addr;
	uint32_t value;
	char expr[32];
	struct breakpoint *next;

	/* TODO: Add more members if necessary */


} BP;

enum _bp_state{ NORMAL, RECOVER };
extern enum _bp_state bp_state;
extern swaddr_t bp_backup;

void free_bp(int no);
void free_all();
void add_bp(swaddr_t addr);
void reset_bp(swaddr_t addr);
void restore_bp(swaddr_t adddr);
BP* getHead();
#endif
