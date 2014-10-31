#include "exec/helper.h"
#include "cpu/modrm.h"

typedef int (*helper_fun)(swaddr_t);
extern helper_fun concat(group_table_, OPCODE)[8];

make_helper(concat(group_, OPCODE)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	return concat(group_table_, OPCODE)[m.reg](eip);
}
