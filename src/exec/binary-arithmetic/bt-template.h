#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(bt_r2rm_, SUFFIX))
{
	int len = 2;

	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);

	DATA_TYPE src = REG(m.reg);
	DATA_TYPE dest;

	if (m.mod == 3) {
		dest = REG(m.R_M);
		print_asm("bt" str(SUFFIX) " %%%s,%%%s", 
				REG_NAME(m.reg), REG_NAME(m.R_M));
		len ++;
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 2, &addr);
		dest = MEM_R(addr);
		print_asm("bt" str(SUFFIX) " %%%s,%s",
				REG_NAME(m.reg), ModR_M_asm);
	}

	FLAG_CHG(CF, ((dest >> src ) & 0x1));

	return len;
}

#include "exec/template-end.h"
