#include "exec/helper.h"
#include "cpu/modrm.h"

#include "nemu.h"

int exec(swaddr_t);

typedef int (*helper_fun)(swaddr_t);
extern helper_fun opcode_table_2[256];

char suffix = 'l';

make_helper(data_size) {
	suffix = 'w';
	int instr_len = exec(eip + 1);
	suffix = 'l';
	return instr_len + 1;
}

make_helper(opcode_2) {
	return opcode_table_2[ instr_fetch(eip + 1, 1) ](eip);
}
