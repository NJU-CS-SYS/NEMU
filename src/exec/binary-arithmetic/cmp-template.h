#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(cmp_i8_rm_, SUFFIX)) {
	ModR_M m;
	uint8_t imm;
	uint32_t len = 1; // 1 for imm8
	DATA_TYPE sub;
	DATA_TYPE result;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
		len += 1;
		sub = REG(m.R_M);
		print_asm("cmp" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
	}
	else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		sub = MEM_R(addr);
		print_asm("cmp" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
	}
	TEMP_SUB_I(imm, sub, result);
	return len + 1; // 1 for opcode
}

#include "exec/template-end.h"
