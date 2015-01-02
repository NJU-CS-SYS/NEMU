#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(exec);

make_helper(concat(rep_, SUFFIX)) {
	int len = 0;
	Log("rep");
	uint8_t next_opcode = instr_fetch(eip + 1, 1);
	if (next_opcode == 0xc3) { // repz ret
		return 1 + exec(eip + 1);
	}
	while (REG(R_ECX) != 0) {
		len = exec(eip+1);
		REG(R_ECX) --;
	}
	return len + 1;
}

#include "exec/template-end.h"
