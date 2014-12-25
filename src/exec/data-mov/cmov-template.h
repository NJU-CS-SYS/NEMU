#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "cc.h"

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

make_helper(concat(cmovg_, SUFFIX))    { make_cmov(CC_G, g, SUFFIX); }
make_helper(concat(cmovge_, SUFFIX))   { make_cmov(CC_GE, ge, SUFFIX); }
make_helper(concat(cmove_, SUFFIX))    { make_cmov(CC_E, e, SUFFIX); }
make_helper(concat(cmovne_, SUFFIX))   { make_cmov(CC_NE, ne, SUFFIX); }
make_helper(concat(cmovb_, SUFFIX))    { make_cmov(CC_B, b, SUFFIX); }
make_helper(concat(cmovbe_, SUFFIX))   { make_cmov(CC_BE, be, SUFFIX); }
make_helper(concat(cmovcxz_, SUFFIX))  { make_cmov(CC_CXZ, cxz, SUFFIX); }
make_helper(concat(cmovecxz_, SUFFIX)) { make_cmov(CC_ECXZ, ecxz, SUFFIX); }
make_helper(concat(cmovl_, SUFFIX))    { make_cmov(CC_L, l, SUFFIX); }
make_helper(concat(cmovle_, SUFFIX))   { make_cmov(CC_LE, le, SUFFIX); }
make_helper(concat(cmova_, SUFFIX))    { make_cmov(CC_A, a, SUFFIX); }
make_helper(concat(cmovae_, SUFFIX))   { make_cmov(CC_AE, ae, SUFFIX); }
make_helper(concat(cmovna_, SUFFIX))   { make_cmov(CC_NA, na, SUFFIX); }
make_helper(concat(cmovnae_, SUFFIX))  { make_cmov(CC_NAE, nae, SUFFIX); }
make_helper(concat(cmovnb_, SUFFIX))   { make_cmov(CC_NB, nb, SUFFIX); }
make_helper(concat(cmovnbe_, SUFFIX))  { make_cmov(CC_NBE, nbe, SUFFIX); }
make_helper(concat(cmovng_, SUFFIX))   { make_cmov(CC_NG, ng, SUFFIX); }
make_helper(concat(cmovnge_, SUFFIX))  { make_cmov(CC_NGE, nge, SUFFIX); }
make_helper(concat(cmovnl_, SUFFIX))   { make_cmov(CC_NL, nl, SUFFIX); }
make_helper(concat(cmovnel_, SUFFIX))  { make_cmov(CC_NEL, nel, SUFFIX); }
make_helper(concat(cmovno_, SUFFIX))   { make_cmov(CC_NO, no, SUFFIX); }
make_helper(concat(cmovnp_, SUFFIX))   { make_cmov(CC_NP, np, SUFFIX); }
make_helper(concat(cmovns_, SUFFIX))   { make_cmov(CC_NS, ns, SUFFIX); }
make_helper(concat(cmovnz_, SUFFIX))   { make_cmov(CC_NZ, nz, SUFFIX); }
make_helper(concat(cmovo_, SUFFIX))    { make_cmov(CC_O, o, SUFFIX); }
make_helper(concat(cmovp_, SUFFIX))    { make_cmov(CC_P, p, SUFFIX); }
make_helper(concat(cmovs_, SUFFIX))    { make_cmov(CC_S, s, SUFFIX); }
#include "exec/template-end.h"
