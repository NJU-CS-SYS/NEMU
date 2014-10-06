#ifndef __UI_BREAKPOINT_H__
#define __UI_BREAKPOINT_H__

#include "common.h"

#define INT3_CODE 0xcc
#define NR_BP 32
typedef struct breakpoint {
	int NO;
	swaddr_t addr;                 // use only for breakpoint
	uint32_t value;                // use both for breakpoint and watchpoint
	char expr[32];                 // use only for watchpoint
	struct breakpoint *next;
} BP;

enum _bp_state{ NORMAL, RECOVER };
enum _wp_state{ ON, OFF };

extern int bp_state;
extern int wp_state;
extern swaddr_t bp_backup;

void free_bp(int no);
void free_all();
void add_bp(swaddr_t addr);
void reset_bp(swaddr_t addr);
void restore_bp(swaddr_t adddr);
BP* getHead();
void add_watchpoint(char* expr);
bool check_watchpoint(int result[], int* nr_changed);
void print_watchpoint(const int result[], const int nr_changed);
#endif
