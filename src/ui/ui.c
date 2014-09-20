#include "ui/ui.h"

#include "nemu.h"

#include <signal.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int nemu_state = END;

void cpu_exec(uint32_t);
void restart();

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
			scanf("%c", &c);
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

static void cmd_si(int run_times) {
	if (nemu_state == END) {
		restart();
	}
	nemu_state = RUNNING;
	cpu_exec(run_times);
	if(nemu_state != END) { nemu_state = STOP; }
}

static void cmd_info(char* opt) {
	if (strcmp(opt, "r") == 0) {
		printf("%-15s%-15X%u\n","eax", cpu.eax, cpu.eax);
		printf("%-15s%-15X%u\n","ecx", cpu.ecx, cpu.ecx); 
		printf("%-15s%-15X%u\n","edx", cpu.edx, cpu.edx); 
		printf("%-15s%-15X%u\n","ebx", cpu.ebx, cpu.ebx); 
		printf("%-15s%-15X%u\n","esp", cpu.esp, cpu.esp); 
		printf("%-15s%-15X%u\n","ebp", cpu.ebp, cpu.ebp); 
		printf("%-15s%-15X%u\n","esi", cpu.esi, cpu.esi); 
		printf("%-15s%-15X%u\n","edi", cpu.edi, cpu.edi); 
		printf("%-15s%-15X%u\n","eip", cpu.eip, cpu.eip); 
	}
}	
void main_loop() { /* oh, main loop ! */
	char *cmd;
	while(1) {
		cmd = rl_gets();
		char *p = strtok(cmd, " ");/* use strtok to break a line into fiels with ' '*/	

		if(p == NULL) { continue; }

		if(strcmp(p, "c") == 0) { cmd_c(); }
		else if(strcmp(p, "r") == 0) { cmd_r(); }
		else if(strcmp(p, "q") == 0) { return; }
		else if(strcmp(p, "si") == 0) {
			p = strtok(NULL," ");
			if (p == NULL) { cmd_si(1); }
			else { cmd_si(atoi(p)); }
		}
		else if(strcmp(p, "info") == 0) { cmd_info(strtok(NULL, " ")); }
		/* TODO: Add more commands */

		else { printf("Unknown command '%s'\n", p); }
	}
}
