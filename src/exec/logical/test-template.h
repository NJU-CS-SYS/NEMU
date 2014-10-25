#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(test_r_rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	DATA_TYPE AND = REG(m.reg);
	uint32_t len = 1;
	if(m.mod == 3) {
		AND &= REG(m.R_M);
		print_asm("test" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		AND &= MEM_R(addr);
		print_asm("test" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}
	FLAG_CHG(CF, 0);
	FLAG_CHG(OF, 0);
	FLAG_CHG(SF, MSB(AND));
	FLAG_CHG(ZF, AND==0);
	FLAG_CHG(PF, parity(AND));
	return len + 1;
}
#include "exec/template-end.h"
