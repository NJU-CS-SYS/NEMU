#include "ui/ui.h"

#include "nemu.h"

#include <setjmp.h>

#define LOADER_START 0x100000

int sig_test = 0;

int exec(swaddr_t);
void load_prog();
void init_dram();
void tlb_init();

char assembly[40];
char *asm_string = (char *)assembly;
jmp_buf jbuf;	/* Make it easy to perform exception handling */

extern uint8_t loader [];
extern uint32_t loader_len;
extern int quiet;

static int trigger = 0;
void restart() {
	/* Perform some initialization to restart a program */
	load_prog();
	memcpy(hwa_to_va(LOADER_START), loader, loader_len);
	tlb_init();

	/* General initialization */
	cpu.eip = LOADER_START;
	cpu.ebp = 0;
	cpu.esp = 0x8000000;
	cpu.eflags = 0x2;
	cpu.eax = 0;
	cpu.ecx = 0;
	cpu.edx = 0;
	cpu.cr0.paging = 0;

	/* Segment initialization */
	cpu.gdtr.limit = 0;
	cpu.gdtr.base = 0;
	cpu.cr[0] = 0; // Set PE to 0

	init_dram();

	/* Enable the breakpoint */
	bp_state = INIT;

	trigger = 0;
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

	setjmp(jbuf);

	for(; n > 0; n --) {
		/* enable the breakpoint when enter loaded program */
		if (cpu.eip > 0x800000 && bp_state == INIT) {
			enable_all_bp();
			bp_state = NORMAL;
		}
		swaddr_t eip_temp = cpu.eip;
		int instr_len = exec(cpu.eip);

		/* restore the breakpoint on the byte */
		if (bp_state == RECOVER) { 
			swaddr_write(bp_backup, 1, INT3_CODE);
			bp_state = NORMAL;
		}
		
		if (cpu.eip == 0xc0100eb2) nemu_state = TEST_INT;
		cpu.eip += instr_len;

		if(n_temp != -1 || (enable_debug && !quiet)) {
			print_bin_instr(eip_temp, instr_len);
			puts(assembly);

		}

		if (wp_state == ON) {
			int result[NR_BP] = { 0 };
			int nr_changed;
			if (check_watchpoint(result, &nr_changed)) {
				print_watchpoint(result, nr_changed);	
				return;
			}
		}
		
		switch(nemu_state) {
			case INT:
				printf("\n\nUser interrupt\n");
				restore_bp(--cpu.eip);
				bp_state = RECOVER;
				bp_backup = cpu.eip;
				return;
			case END:
				free_all();
				return;
			case TEST_INT:
				return;
		}
	}
}
