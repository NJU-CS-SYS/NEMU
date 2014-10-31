#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(add_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE src, dest, result;
	src = REG(m.reg);
	if (m.mod == 3) {
		dest = REG(m.R_M);
		TEMP_ADD_I(src, dest, result);
		REG(m.R_M) = result;
		print_asm("add" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 1 + 1;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		dest = MEM_R(addr);
		TEMP_ADD_I(src, dest, result);
		MEM_W(addr, result);
		print_asm("add" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return 1 + len;
	}
}

#include "exec/template-end.h"
