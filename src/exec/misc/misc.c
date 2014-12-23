#include "exec/helper.h"
#include "ui/ui.h"
#include "nemu.h"
#include "cpu/intr.h"

void print_bin_instr(swaddr_t eip, int len);
extern char assembly[];

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
	char buf[1024];
	int i, limit;
	switch (cpu.eax) {
		case 4:
		    limit = cpu.edx;
			for (i = 0; i < limit; i ++)
				buf[i] = swaddr_read(cpu.ecx + i, 1);
			buf[i] = '\0';
			puts(buf);
			break;
		default:
			printf("nemu: HIT \33[1;31m%s\33[0m TRAP at eip = 0x%08x\n\n", (cpu.eax == 0 ? "GOOD" : "BAD"), cpu.eip);
			nemu_state = END;
	}

	print_asm("nemu trap");
	return 1;
}
