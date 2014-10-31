#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define SET_CONDITION(x) \
int len = 2;\
ModR_M m;\
m.val = instr_fetch(eip + 2, 1);\
if (m.mod == 3) {\
	REG(m.reg) = (x);\
	return len + 1;\
} else {\
	swaddr_t addr;\
	len += read_ModR_M(eip + 2, &addr);\
	MEM_W(addr, (x));\
	return len;\
}

make_helper(set_ne_nz) {
	SET_CONDITION(FLAG_VAL(ZF));
}

#include "exec/template-end.h"
