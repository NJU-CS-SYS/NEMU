#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "template.h"

make_helper(concat(sub_i2r_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	int reg_code;
	switch (DATA_BYTE) {
		case 1: reg_code = R_AL; break;
		case 2: reg_code = R_AX; break;
		case 4: reg_code = R_EAX; break;
		default: reg_code = R_EAX; assert(0);
	}
	DATA_TYPE dest = REG(reg_code);
	DATA_TYPE result = dest;
	TEMP_SUB_I(imm, dest, result);
	REG(reg_code) = result;

	print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}


make_helper(concat(sub_i2rm_, SUFFIX)) {
	/*
	ModR_M m;
	DATA_TYPE imm;
	DATA_TYPE dest;
	DATA_TYPE result;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		dest = REG(m.R_M);
		result = dest;
		TEMP_SUB_I(imm, dest, result);
		print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		dest = MEM_R(addr);
		result = dest;
		TEMP_SUB_I(imm, dest, result);
		MEM_W(addr, result);
		print_asm("sub" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
	*/
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, DATA_BYTE);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sub_i82rm_, SUFFIX)) {
	/*
	ModR_M m;
	DATA_TYPE imm;
	DATA_TYPE dest;
	DATA_TYPE result;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, 1);
		dest = REG(m.R_M);
		result = dest;
		TEMP_SUB_I(imm, dest, result);
		print_asm("sub" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + 1  + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, 1);
		dest = MEM_R(addr);
		result = dest;
		TEMP_SUB_I(imm, dest, result);
		MEM_W(addr, result);
		print_asm("sub" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + 1 + 1;
	}
	*/
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, 1);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sub_r2rm_, SUFFIX)) {
	TEMP_VALUES;
	TEMP_MOD_RM;
	TEMP_R2RM(sub);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}
/*
make_helper(concat(mov_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.R_M) = REG(m.reg);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		MEM_W(addr, REG(m.reg));

		print_asm("mov" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(mov_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.reg) = REG(m.R_M);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		REG(m.reg) = MEM_R(addr);

		print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return len + 1;
	}
}

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}
*/
#include "exec/template-end.h"
