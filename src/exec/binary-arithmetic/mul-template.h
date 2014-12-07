#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

#define CLEAR_FLAG do{\
	FLAG_CHG(OF, 0);\
	FLAG_CHG(CF, 0);\
}while(0)

#if DATA_BYTE == 1
#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_AH) != 0);\
	FLAG_CHG(CF, REG(R_AH) != 0);\
}while(0)

#define IMUL_FLAG(rst) do{\
	FLAG_CHG(OF, (((rst) & 0xff00ULL) != 0));\
	FLAG_CHG(CF, (((rst) & 0xff00ULL) != 0));\
}while(0)

#define MUL_RST(rst) do{\
	reg_w(R_AX) = rst;\
}while(0)

#define IMUL_RST(rst) do{\
	reg_b(R_AL) = rst & 0xff;\
	reg_b(R_AH) = (rst << 56) >> 60;\
}while(0)

#elif DATA_BYTE == 2

#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_DX) != 0);\
	FLAG_CHG(CF, REG(R_DX) != 0);\
}while(0)

#define IMUL_FLAG(rst) do{\
	FLAG_CHG(OF, (((rst) & 0xffff0000ULL) != 0));\
	FLAG_CHG(CF, (((rst) & 0xffff0000ULL) != 0));\
}while(0)

#define MUL_RST(rst) do{\
	reg_w(R_DX) = (DATA_TYPE)(rst >> 16);\
	reg_w(R_AX) = (DATA_TYPE)(rst);\
}while(0)

#define IMUL_RST(rst) do{\
	reg_w(R_DX) = (rst) >> 16;\
	reg_w(R_AX) = (rst);\
}while(0)

#elif DATA_BYTE == 4
#define MUL_FLAG do{\
	FLAG_CHG(OF, REG(R_EDX) != 0);\
	FLAG_CHG(CF, REG(R_EDX) != 0);\
}while(0)

#define IMUL_FLAG(rst) do{\
	FLAG_CHG(OF, (((rst) & 0xffffffff00000000ULL) != 0));\
	FLAG_CHG(CF, (((rst) & 0xffffffff00000000ULL) != 0));\
}while(0)

#define MUL_RST(rst) do {\
	reg_l(R_EDX) = (DATA_TYPE)(rst >> 32);\
	reg_l(R_EAX) = (DATA_TYPE)(rst);\
}while(0)

#define IMUL_RST(rst) do {\
	reg_l(R_EDX) = (rst) >> 32;\
	reg_l(R_EAX) = (rst);\
}while(0)
#endif

#define LOG do{\
	Log("multiplier = %d multiplicat = %d result = %d", (int)src, (int)dst, (int)rst);\
}while(0)

make_helper(concat(mul_rm2r_, SUFFIX)) {
	uint64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	dst = REG(R_EAX);
	MOD_RM2R(mul, src, len);
	rst = src * dst;
	MUL_FLAG;
	MUL_RST(rst);
	LOG;
	return len + 1;
}

make_helper(concat(imul_rm2imp_, SUFFIX)) {
	int64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	dst = REG(R_EAX);
	m.val = instr_fetch(eip + 1, 1);
	if (m.mod == 3) {
		src = REG(m.R_M);
		len++;
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 1, &addr);
		src = MEM_R(addr);
	}
	rst = src * dst;
	CLEAR_FLAG;
	IMUL_RST(rst);
	LOG;

	if (m.mod == 3) print_asm("imul" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
	else print_asm("imul" str(SUFFIX) " %s", ModR_M_asm);

	return len;
}

make_helper(concat(imul_rm2r_, SUFFIX)) {
	eip++; // 2 bytes opcode !
	int64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = REG(m.reg);
	rst = src * dst;
	REG(m.reg) = rst;
	CLEAR_FLAG;
	LOG;
	return len + 1;
}

make_helper(concat(imul_i8rm2r_, SUFFIX)) {
	int64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = instr_fetch(eip + len, 1);
	dst = (dst << 56) >> 56; // sign-extended
	rst = src * dst;
	REG(m.reg) = rst;
	IMUL_FLAG(rst);
	LOG;
	return len + 1;
}

make_helper(concat(imul_irm2r_, SUFFIX)) {
	int64_t src, dst, rst;
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = instr_fetch(eip + len, DATA_BYTE);
	rst = src * dst;
	REG(m.reg) = rst;
	CLEAR_FLAG;
	LOG;
	return len + DATA_BYTE;
}
#undef MUL_FLAG
#undef MUL_RST
#undef IMUL_FLAG
#undef IMUL_RST
#include "exec/template-end.h"
