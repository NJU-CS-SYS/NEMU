#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

#define SHIFT_HEAD_IMM \
int imm;\
int len = 0;\
int reg_code = 8;\
swaddr_t addr = 0;\
DATA_TYPE dest;\
ModR_M m;\
m.val = instr_fetch(eip + 1, 1);\
if (m.mod == 3) {\
	reg_code = m.R_M;\
	dest = REG(reg_code);\
	imm = instr_fetch(eip + 1 + 1, 1);\
	len = 3;\
} else {\
	len += read_ModR_M(eip + 1, &addr);\
	dest = MEM_R(addr);\
	imm = instr_fetch(eip + len + 1, 1);\
	len += 1 + 1;\
}\

#define SHIFT_PROCESS(direct, type, dest, times) \
do {\
	int temp = times;\
	while (temp != 0) {\
		if (direct == 'l') {\
			FLAG_CHG(CF, MSB(dest));\
			dest <<= 1;\
		} else if (type == 'i') {\
			FLAG_CHG(CF, LSB(dest));\
			dest = (DATA_TYPE_S)dest >> 1;\
		} else if (type == 'u') {\
			FLAG_CHG(CF, LSB(dest));\
			dest = dest >> 1;\
		} else {\
			test(0, "Unexpect case");\
		}\
		temp--;\
	}\
	if (times == 1) {\
		if (direct == 'l') {\
			FLAG_CHG(OF, (MSB(dest) != (FLAG_VAL(CF))));\
		} else if (type == 'i') {\
			FLAG_CHG(OF, 0);\
		} else if (type == 'u') {\
			FLAG_CHG(OF, MSB(dest));\
		}\
	}\
} while (0)

#define SHIFT_PRINT(name) \
(reg_code < 8) ? print_asm(str(name) str(SUFFIX) " %%%s", REG_NAME(reg_code)) : print_asm(str(name) str(SUFFIX) " %s", ModR_M_asm);

make_helper(concat(sal_i8_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sal, 1);
	SHIFT_PROCESS('l', 'i', dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
}
make_helper(concat(sar_i8_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sar, 1);
	SHIFT_PROCESS('r', 'i', dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
}

make_helper(concat(sal_12rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	test(m.reg == 4, "wrong dispatching");
	if (m.mod == 3) {
		dest = REG(m.R_M);
		print_asm("sal" str(SUFFIX) " $1,%%%s", REG_NAME(m.R_M));
		len++;
	} else {
		len += read_ModR_M(eip + 1, &addr);
		dest = MEM_R(addr);
		print_asm("sal" str(SUFFIX) " $1,%s", ModR_M_asm);
	}
	src = 1;
	SHIFT_PROCESS('l', 'i', dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
} 
make_helper(concat(sar_12rm_, SUFFIX)) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	test(m.reg == 4, "wrong dispatching");
	if (m.mod == 3) {
		dest = REG(m.R_M);
		print_asm("sal" str(SUFFIX) " $1,%%%s", REG_NAME(m.R_M));
		len++;
	} else {
		len += read_ModR_M(eip + 1, &addr);
		dest = MEM_R(addr);
		print_asm("sal" str(SUFFIX) " $1,%s", ModR_M_asm);
	}
	src = 1;
	SHIFT_PROCESS('r', 'i', dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
} 


#include "exec/template-end.h"
