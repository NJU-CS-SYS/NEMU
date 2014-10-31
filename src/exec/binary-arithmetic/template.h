#ifndef __TEMPLATE__
#define __TEMPLATE__
#include "exec/helper.h"
//#include "exec/flag.h"
#define _TEMP_SUB_I(src, dest, result)\
if (sizeof(src) == 1 && sizeof(dest) > 1) {\
	result = dest - (DATA_TYPE_S)src;\
} else {\
	result = dest - src;\
}

#define TEMP_FLAG(src, dest, result)\
FLAG_CHG(OF,OVERFLOW(src, dest, result));\
FLAG_CHG(SF, MSB(result));\
FLAG_CHG(ZF, result==0);\
FLAG_CHG(AF, ADJUST(src, dest));\
FLAG_CHG(PF, PARITY(result));\
FLAG_CHG(CF, result < dest)

#define TEMP_ADD_I(src, dest, result)\
result = dest + (DATA_TYPE_S)src;\
TEMP_FLAG(src, dest, result)

#define TEMP_SUB_I(src, dest, result) \
_TEMP_SUB_I(src, dest, result);\
TEMP_FLAG(src, dest, result)

#define TEMP_R2RM(name) \
if (m.mod == 3) {\
	dest = REG(m.R_M);\
	len++;\
	print_asm(str(name) str(SUFFIX) " %%%s, %%%s", REG_NAME(m.reg), REG_NAME(m.R_M));\
} else {\
	len += read_ModR_M(eip + 1, &addr);\
	dest = MEM_R(addr);\
	print_asm(str(name) str(SUFFIX) " %%%s, %s", REG_NAME(m.reg), ModR_M_asm);\
}	

#define TEMP_VALUES \
DATA_TYPE src, dest, result;\
int len = 1

#define TEMP_MOD_RM \
ModR_M m;\
m.val = instr_fetch(eip + 1, 1);\
swaddr_t addr

#endif
