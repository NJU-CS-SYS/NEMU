#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

#ifndef __MUL_TYPE__
#define __MUL_TYPE__
typedef union __mul16_t__ {
	struct {
		uint8_t low;
		uint8_t high;
	};
	uint16_t val;
} mul16_t;
typedef union __mul32_t__ {
	struct {
		uint16_t low;
		uint16_t high;
	};
	uint32_t val;
}mul32_t;
typedef union __mul64_t__ {
	struct {
		uint32_t low;
		uint32_t high;
	};
	uint64_t val;
}mul64_t;
typedef union __imul16_t__ {
	struct {
		uint8_t low;
		int8_t high;
	};
	int16_t val;
}imul16_t;
typedef union __imul32_t__ {
	struct {
		uint16_t low;
		int16_t high;
	};
	int32_t val;
}imul32_t;
typedef union __imul64_t__ {
	struct {
		uint32_t low;
		int32_t high;
	};
	uint64_t val;
}imul64_t;
#endif

// type define macro
#if DATA_BYTE == 1
#define RST_TYPE   mul16_t
#define RST_TYPE_S imul16_t
#elif DATA_BYTE == 2
#define RST_TYPE   mul32_t
#define RST_TYPE_S imul32_t
#else
#define RST_TYPE   mul64_t
#define RST_TYPE_S imul64_t
#endif

// flag control macro
#define CLEAR_FLAG \
do{\
	FLAG_CHG(OF, 0);\
	FLAG_CHG(CF, 0);\
}while(0)

#define MUL_FLAG(rst) \
do{\
	FLAG_CHG(OF, rst.high != 0);\
	FLAG_CHG(CF, rst.high != 0);\
}while(0)

#if DATA_BYTE == 1
#define RST_H R_AH
#define RST_L R_AL
#elif DATA_BYTE == 2
#define RST_H R_DX
#define RST_L R_AX
#elif DATA_BYTE == 4
#define RST_H R_EDX
#define RST_L R_EAX
#endif

#define MUL_RST(rst)\
	do {\
		REG(RST_H) = rst.high;\
		REG(RST_L) = rst.low;\
	} while (0);

make_helper(concat(mul_rm2r_, SUFFIX)) {
	DATA_TYPE src, dst;
	RST_TYPE rst;
	int len = 1;
	ModR_M m;
	dst = REG(R_EAX);
	MOD_RM2R(mul, src, len);
	rst.val = src * dst;
	MUL_FLAG(rst);
	MUL_RST(rst);
	return len + 1;
}

make_helper(concat(imul_rm2imp_, SUFFIX)) {
	RST_TYPE_S rst;
	DATA_TYPE_S src, dst; 
	int len = 1; ModR_M m;
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
	rst.val = src * dst;
	CLEAR_FLAG;
	MUL_RST(rst);

	Log("Src %x * Dst %x = High %x Low %x", src, dst, rst.high, rst.low);
	if (m.mod == 3) print_asm("imul" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
	else print_asm("imul" str(SUFFIX) " %s", ModR_M_asm);

	return len;
}

make_helper(concat(imul_rm2r_, SUFFIX)) {
	eip++; // 2 bytes opcode !
	RST_TYPE_S rst;
	DATA_TYPE_S src, dst; 
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = REG(m.reg);
	rst.val = src * dst;
	REG(m.reg) = (DATA_TYPE_S)rst.val;
	CLEAR_FLAG;
	return len + 1;
}

make_helper(concat(imul_i8rm2r_, SUFFIX)) {
	RST_TYPE_S rst;
   	DATA_TYPE_S src, dst;
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = instr_fetch(eip + len, 1);
	// sign-extended
	dst = (dst << ((DATA_BYTE - 1) << 3)) >> ((DATA_BYTE - 1) << 3);
	rst.val = src * dst;
	REG(m.reg) = (DATA_TYPE_S)rst.val;
	MUL_FLAG(rst);
	return len + 1;
}

make_helper(concat(imul_irm2r_, SUFFIX)) {
	RST_TYPE_S rst;
	DATA_TYPE_S src, dst;
	int len = 1;
	ModR_M m;
	MOD_RM2R(imul, src, len);
	dst = instr_fetch(eip + len, DATA_BYTE);
	rst.val = src * dst;
	REG(m.reg) = (DATA_TYPE_S)rst.val;
	CLEAR_FLAG;
	return len + DATA_BYTE;
}
#undef MUL_FLAG
#undef MUL_RST
#undef IMUL_FLAG
#undef RST_TYPE
#undef RST_TYPE_S
#undef RST_L
#undef RST_H
#include "exec/template-end.h"
