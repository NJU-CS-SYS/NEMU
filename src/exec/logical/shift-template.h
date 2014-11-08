#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

#define SHIFT_PROCESS(dest, times) \
do {\
	int temp = times;\
	while (temp != 0) {\
		if (DIRTAG == 0) {\
			FLAG_CHG(CF, MSB(dest));\
			dest <<= 1;\
		} else if (SIGNTAG == 1) {\
			FLAG_CHG(CF, LSB(dest));\
			dest = (DATA_TYPE_S)dest >> 1;\
		} else if (SIGNTAG == 0) {\
			FLAG_CHG(CF, LSB(dest));\
			dest = dest >> 1;\
		} else {\
			test(0, "Unexpect case");\
		}\
		temp--;\
	}\
	if (times == 1) {\
		if (DIRTAG == 0) {\
			FLAG_CHG(OF, (MSB(dest) != (FLAG_VAL(CF))));\
		} else if (SIGNTAG == 1) {\
			FLAG_CHG(OF, 0);\
		} else if (SIGNTAG == 0) {\
			FLAG_CHG(OF, MSB(dest));\
		} else {\
			test(0, "Unexpect flag case");\
		}\
	}\
} while (0)

#define make_shift(mod) make_helper(concat(concat(s, concat(SIGN, concat(DIR, _##mod##_))), SUFFIX))
//make_helper(concat(concat(s, concat(SIGN, concat(DIR, _i8_))), SUFFIX)) {
make_shift(i8) {
	TEMP_VALUES_S;
	TEMP_MOD_RM;
	TEMP_I2RM(sal, 1);
	SHIFT_PROCESS(dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
}
make_shift(12rm) {
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
	SHIFT_PROCESS(dest, src);
	result = dest;
	TEMP_RESULT2RM(result);
	return len;
}
#include "exec/template-end.h"
