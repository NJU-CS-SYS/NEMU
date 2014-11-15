#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

make_helper(concat(div_rm2imp_, SUFFIX)) {
	uint64_t src, dst , quo, rem;
	int len = 1;
	ModR_M m;
	GET_RM(src, len);

	int quo_reg, rem_reg;
#if DATA_BYTE == 1
	dst = REG(R_AX);
	quo_reg = R_AH;
	rem_reg = R_AL;
#elif DATA_BYTE == 2
	dst = REG(R_DX);
	dst <<= 16;
	dst = dst | REG(R_AX);
	quo_reg = R_AX;
	rem_reg = R_DX;
#elif DATA_BYTE == 4
	dst = REG(R_EDX);
	dst <<= 32;
	dst = dst | REG(R_EAX);
	quo_reg = R_EAX;
	rem_reg = R_EDX;
#endif

	quo = dst / src;
	rem = dst % src;
	REG(quo_reg) = quo;
	REG(rem_reg) = rem;

	if (m.mod == 3)
		print_asm("div" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
	else
		print_asm("div" str(SUFFIX) " %s", ModR_M_asm);

	return len;
}

make_helper(concat(idiv_rm2imp_, SUFFIX)) {
	int64_t src, dst , quo, rem;
	int len = 1;
	ModR_M m;
	GET_RM(src, len);

	int quo_reg, rem_reg;
#if DATA_BYTE == 1
	dst = REG(R_AX);
	quo_reg = R_AH;
	rem_reg = R_AL;
#elif DATA_BYTE == 2
	dst = REG(R_DX);
	dst <<= 16;
	dst = dst | REG(R_AX);
	quo_reg = R_AX;
	rem_reg = R_DX;
#elif DATA_BYTE == 4
	dst = REG(R_EDX);
	dst <<= 32;
	dst = dst | REG(R_EAX);
	quo_reg = R_EAX;
	rem_reg = R_EDX;
#endif

	quo = dst / src;
	rem = dst % src;
	REG(quo_reg) = quo;
	REG(rem_reg) = rem;

	if (m.mod == 3)
		print_asm("div" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
	else
		print_asm("div" str(SUFFIX) " %s", ModR_M_asm);

	return len;
}
#include "exec/template-end.h"
