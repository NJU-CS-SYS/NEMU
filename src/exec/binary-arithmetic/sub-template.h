/* Module : sub
 * State  : Finished
 * Date   : 2014/11/02 20:50
 */
#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

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
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, DATA_BYTE);
	TEMP_SUB_I(src, dest, result);
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sub_i82rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sub, 1);
	int shift_len = (DATA_BYTE - 1) * 8;
	src = (src << shift_len) >> shift_len;
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

make_helper(concat(sub_rm2r_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_RM2R(sub);
	TEMP_SUB_I(dest, src, result);
	REG(m.reg) = result;
	return len;
}

make_helper(concat(neg_rm_, SUFFIX)) {
	ModR_M m;
	int len = 1;
	int buf;
	m.val = instr_fetch(eip + 1, 1);
	test(m.reg == 3, "wrong dispatch");
	if (m.mod == 3) {
		FLAG_CHG(CF, REG(m.R_M) != 0);
		buf = -REG(m.R_M);
		REG(m.R_M) = buf;
		print_asm("neg" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
		len ++;
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		FLAG_CHG(CF, MEM_R(addr));
		buf = -MEM_R(addr);
		MEM_W(buf, addr);
		print_asm("neg" str(SUFFIX) " %s", ModR_M_asm);
	}
	FLAG_CHG(OF, buf == 0x80000000);
	FLAG_CHG(SF, MSB(buf));
	FLAG_CHG(ZF, buf == 0);
	FLAG_CHG(PF, PARITY(buf));
	return len;
}
#include "exec/template-end.h"
