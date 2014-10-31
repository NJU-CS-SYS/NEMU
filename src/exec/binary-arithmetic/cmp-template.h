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

make_helper(concat(cmp_r2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE src, dest, result;
	int len = 1;

	m.val = instr_fetch(eip + 1, 1);
	src = REG(m.reg);
	
	if (m.mod == 3) {
		dest = REG(m.R_M);
		len++;
		print_asm("cmp" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		dest = MEM_R(addr);
		print_asm("cmp" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
	}

	TEMP_SUB_I(src, dest, result);
	return len;
}

#include "exec/template-end.h"
