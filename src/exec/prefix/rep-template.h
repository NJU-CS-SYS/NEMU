#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(exec);

make_helper(concat(rep_, SUFFIX)) {
	uint8_t next_opcode = instr_fetch(eip + 1, 1);

	if (next_opcode == 0xc3) { // repz ret
		return 1;  // don't count rep, for right eip
	}

	while (cpu.ecx != 0) {
		Log("rep %x", REG(R_ECX));
		exec(eip+1);
		cpu.ecx --;
	}
	return 2;
}

#include "exec/template-end.h"
