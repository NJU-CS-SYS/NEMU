#include "ui/ui.h"
#include "ui/breakpoint.h"

#include "nemu.h"


#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int nemu_state = END;
int bp_state = NORMAL;
int wp_state = OFF;
swaddr_t bp_backup = 0;

void cpu_exec(uint32_t);
void restart();
void test_tokens(char* e);
uint32_t calculate(char* e);

/* We use the readline library to provide more flexibility to read from stdin. */
char* rl_gets() { /* read line get string */
	static char *line_read = NULL;

	if (line_read) { /* why this statements needed? */
		free(line_read);
		line_read = NULL;
	}

	/* now line_read must be NULL */

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	/* line_read points to sth and the string isn't \0 */

	return line_read;
}

/* This function will be called when you press <C-c>. And it will return to 
 * where you press <C-c>. If you are interesting in how it works, please
 * search for "Unix signal" in the Internet.
 */
static void control_C(int signum) {
	if(nemu_state == RUNNING) {
		nemu_state = INT;
	}
}

void init_signal() {
	/* Register a signal handler. */
	struct sigaction s;
	/* sigaction is a system struct ! */
	memset(&s, 0, sizeof(s));
	s.sa_handler = control_C;
	int ret = sigaction(SIGINT, &s, NULL);
	assert(ret == 0);
}


static void cmd_c() {
	if(nemu_state == END) {
		puts("The Program does not start. Use 'r' command to start the program.");
		return;
	}

	nemu_state = RUNNING;
	cpu_exec(-1);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_r() {
	if(nemu_state != END) { 
		char c;
		while(1) {
			printf("The program is already running. Restart the program? (y or n)");
			fflush(stdout);
			scanf(" %c",&c);
			switch(c) {
				case 'y': goto restart_;
				case 'n': return;
				default: puts("Please answer y or n.");
			}
		}
	}

restart_:
	restart();
	nemu_state = STOP; /* HIT good trap here !? */
	cmd_c();
}

static void cmd_si() {
	char* p = strtok(NULL, " ");
	int run_times = 1;
	if (p != NULL) { run_times = atoi(p); }
	if (nemu_state == END) {
		restart();
	}
	nemu_state = RUNNING;
	cpu_exec(run_times);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_info() {
	char* opt = strtok(NULL, " ");
	if (strcmp(opt, "r") == 0) {
		printf("%-15s%#-15X%u\n","eax", cpu.eax, cpu.eax);
		printf("%-15s%#-15X%u\n","ecx", cpu.ecx, cpu.ecx); 
		printf("%-15s%#-15X%u\n","edx", cpu.edx, cpu.edx); 
		printf("%-15s%#-15X%u\n","ebx", cpu.ebx, cpu.ebx); 
		printf("%-15s%#-15X%u\n","esp", cpu.esp, cpu.esp); 
		printf("%-15s%#-15X%u\n","ebp", cpu.ebp, cpu.ebp); 
		printf("%-15s%#-15X%u\n","esi", cpu.esi, cpu.esi); 
		printf("%-15s%#-15X%u\n","edi", cpu.edi, cpu.edi); 
		printf("%-15s%#-15X%u\n","eip", cpu.eip, cpu.eip); 
		printf("CF PF AF ZF SF TF IF DF OF\n"
		       "%2d %2d %2d %2d %2d %2d %2d %2d %2d\n",
				FLAG_VAL(CF),
				FLAG_VAL(PF),
				FLAG_VAL(AF),
				FLAG_VAL(ZF),
				FLAG_VAL(SF),
				FLAG_VAL(TF),
				FLAG_VAL(IF),
				FLAG_VAL(DF),
				FLAG_VAL(OF)
		      );
	}
	else if (strcmp(opt, "b") == 0) {
		BP* current = getHead();
		printf("%-6s%-12s%-12s%s\n", "NUM", "TYPE", "ADDRESS", "EXPRESSION");
		while(current != NULL) {
			if (current->expr[0] == '\0') {
				printf("%-6d%-12s%#-12x%s\n", current->NO, "breakpoint", current->addr, current->expr);
			} else {
				printf("%-6d%-12s%#-12x%s\n", current->NO, "watchpoint", current->addr, current->expr);
			}
			current = current->next;
		}
	}
}

void cmd_x() {
	char* _num = strtok(NULL, " ");
	char* _addr = strtok(NULL, "");
	if((_num != NULL) && (_addr != NULL)) {
		int num = atoi(_num);
		swaddr_t addr = calculate(_addr);
		int i;
		for(i = 0; i < num; i++){
			int j;
			printf("%8x:   ", addr+i*4);
			for(j = 0; j < 4; j++) {
			printf("%02x ", swaddr_read(addr+i*4+j, 1));
			}
			putchar('\n');
		}
	}
}

void cmd_b() {
	char* p = strtok(NULL, "");
	if (p[0] == '*') {
		swaddr_t addr = calculate(p+1);
		add_bp(addr);
	}
}

void cmd_d() {
	char *opt = strtok(NULL, " ");
	if (opt == NULL) { 
		free_all(); 
		wp_state = OFF;
	}
	else {
		int no = atoi(opt);
		if (0 <= no && no < NR_BP) free_bp(no);
		else assert(0);
	}
}

void cmd_e() {
	char *expr = strtok(NULL, "");
	test_tokens(expr);
}

void cmd_p() {
	char *e = strtok(NULL, "");
	printf("%u\n", calculate(e));
}

void cmd_w() {
	char *expr = strtok(NULL, "");
	add_watchpoint(expr);
	wp_state = ON;
}
void main_loop() { /* oh, main loop ! */
	char *cmd;
	while(1) {

		cmd = rl_gets();
		char *p = strtok(cmd, " ");/* use strtok to break a line into fiels with ' '*/	

		if(p == NULL) { continue; }

		if(strcmp(p, "q") == 0) { return; }
		else if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "x") == 0) { cmd_x(); }
		else if(strcmp(p, "b") == 0) { cmd_b(); }
		else if(strcmp(p, "d") == 0) { cmd_d(); }
		else if(strcmp(p, "p") == 0) { cmd_p(); }
		else if(strcmp(p, "w") == 0) { cmd_w(); }
		else if(strcmp(p, "si") == 0) { cmd_si(); }
		else if(strcmp(p, "info") == 0) { cmd_info(); }

		/*remember to delete this test instr */
		else if(strcmp(p, "e") == 0) { cmd_e(); }
		else if(strcmp(p, "reload") == 0) { cpu.eip = 0x100000; }

		else { printf("Unknown command '%s'\n", p); }
	}
}
