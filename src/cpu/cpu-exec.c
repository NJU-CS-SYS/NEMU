#include "ui/ui.h"

#include "nemu.h"

#include <setjmp.h>

#define LOADER_START 0x100000

int exec(swaddr_t);
void load_prog();
void init_dram();
void free_all(); /* for breakpoint */

char assembly[40];
jmp_buf jbuf;	/* Make it easy to perform exception handling */

extern uint8_t loader [];
extern uint32_t loader_len;
extern int quiet;

void restart() {
	/* Perform some initialization to restart a program */
	load_prog();
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);

	cpu.eip = LOADER_START;

	init_dram();

	/* clear the breakpoint */
	free_all();
}

static void print_bin_instr(swaddr_t eip, int len) {
	int i;

	printf("%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		printf("%02x ", swaddr_read(eip + i, 1));
	}
	printf("%*.s", 50 - (12 + 3 * len), "");
}

void cpu_exec(volatile uint32_t n) {
	volatile uint32_t n_temp = n;

	/* use volatile means n can never be modified by the func itself but
	 * from memory directly by other func()
	 */

	setjmp(jbuf);
	for(; n > 0; n --) {
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);

		/* restore the breakpoint on the byte */
		if (bp_state == RECOVER) { 
			reset_bp(bp_backup);
			bp_state = NORMAL;
		}
		
		cpu.eip += instr_len;

		if(n_temp != -1 || (enable_debug && !quiet)) {
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);
		}
/*
		if(nemu_state == INT) {
			printf("\n\nUser interrupt\n");
			cpu.eip--;
			return;
		} 
		else if(nemu_state == END) { return; }
*/
		switch(nemu_state) {
			case INT:
				printf("\n\nUser interrupt\n");
				restore_bp(--cpu.eip);
				bp_state = RECOVER;
				bp_backup = cpu.eip;
				return;
			case END:
				return;
		}
	}
}
