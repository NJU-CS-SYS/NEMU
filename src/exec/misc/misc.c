#include "exec/helper.h"
#include "ui/ui.h"
#include "nemu.h"
#include "cpu/intr.h"

void print_bin_instr(swaddr_t eip, int len);
extern char assembly[];
extern int nemu_state;

static char str_buffer[1024];
static int buffer_idx = 0;
static int print_buffer(uint32_t, size_t);

make_helper(inv) {
	/* invalid opcode */

	uint8_t temp[8];
	((uint32_t *)temp)[0] = instr_fetch(eip, 4);
	((uint32_t *)temp)[1] = instr_fetch(eip + 4, 4);
	Log("invalid opcode(eip = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...", 
			eip, temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);

	test(0, "eip %#x", eip);
}

make_helper(int3) {
	/* A breakpoint is hit! Do something here! */
	nemu_state = INT;
	return 1;
}

make_helper(int_i) // opcode = cd
{
	uint8_t imm = instr_fetch(eip + 1, 1);

	/* Special output */
	print_asm("int %#x", imm);
	print_bin_instr(eip, 2);
	puts(assembly);

	raise_intr(imm);
	return 2;
}

make_helper(nemu_trap) {
	switch (cpu.eax) {
		case 2:
			Log("tf->eax %x", cpu.ebx);
			Log("ebp ref %x", cpu.ebp + 4 * 10);
			cpu.eax = print_buffer(cpu.ecx, cpu.edx);
			swaddr_write(cpu.ebp + 4 * 9, 4, cpu.eax);
			break;
		default:
			printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
			nemu_state = END;
	}

	print_asm("nemu trap");
	return 1;
}

static int print_buffer(uint32_t ptr, size_t len)
{
	int i;
	for (i = 0; i < len; i ++) {
		str_buffer[buffer_idx] = swaddr_read(ptr + i, 1);
		if ( str_buffer[buffer_idx] == '\n'  || str_buffer[buffer_idx] == '\0' ) {
			printf("\33[1;32m%s\33[0m", str_buffer);
			fflush(stdout);
			int cnt = strlen(str_buffer);
			memset(str_buffer, 0, 1024);
			buffer_idx = 0;
			nemu_state = TEST_INT;
			return cnt;
		} else {
			buffer_idx ++;
		}
	}
	return -1;
}
