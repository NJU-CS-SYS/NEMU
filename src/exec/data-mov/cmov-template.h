#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

#define CC_G (!FLAG_VAL(ZF))&&(FLAG_VAL(SF)==FLAG_VAL(OF))
#ifndef __CMOV_TEMP_H__
#define __CMOV_TEMP_H__

#define make_cmov(c, NAME, SUFFIX)\
	do{\
		int len = 2;\
		ModR_M m;\
		m.val = instr_fetch(eip + 2, 1);\
		if (m.mod ==  3) {\
			len ++;\
			if ((c)) { \
				REG(m.reg) = REG(m.R_M);\
			}\
			print_asm("cmov" str(NAME) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));\
		} else {\
			swaddr_t addr;\
			len += read_ModR_M(eip + 2, &addr);\
			if ((c)) REG(m.reg) = MEM_R(addr);\
			print_asm("cmov" str(NAME) str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));\
		}\
		return len;\
	}while(0)
#endif

make_helper(concat(cmovg_, SUFFIX)) { make_cmov(CC_G, g, SUFFIX); }

#include "exec/template-end.h"
