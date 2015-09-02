#ifndef __TEMPLATE__
#define __TEMPLATE__
#include "exec/helper.h"

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
FLAG_CHG(CF, (DATA_TYPE)result < (DATA_TYPE)dest)

#define AND_FLAG(src, dest, result)\
FLAG_CHG(OF,0);\
FLAG_CHG(SF, MSB(result));\
FLAG_CHG(ZF, result==0);\
FLAG_CHG(PF, PARITY(result));\
FLAG_CHG(CF, 0)

#define TEMP_ADD_I(src, dest, result)\
result = dest + (DATA_TYPE_S)src;\
TEMP_FLAG(src, dest, result)

#define TEMP_SUB_I(src, dest, result) \
_TEMP_SUB_I(src, dest, result);\
TEMP_FLAG(((~src)+1), dest, result)

#define TEMP_R2RM(name) \
src = REG(m.reg);\
if (m.mod == 3) {\
    dest = REG(m.R_M);\
    len++;\
    print_asm(str(name) str(SUFFIX) " %%%s, %%%s", REG_NAME(m.reg), REG_NAME(m.R_M));\
} else {\
    len += read_ModR_M(eip + 1, &addr);\
    dest = MEM_R(addr);\
    print_asm(str(name) str(SUFFIX) " %%%s, %s", REG_NAME(m.reg), ModR_M_asm);\
}

#define TEMP_RM2R(name) \
src = REG(m.reg);\
if (m.mod == 3) {\
    dest = REG(m.R_M);\
    len++;\
    print_asm(str(name) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));\
} else {\
    len += read_ModR_M(eip + 1, &addr);\
    dest = MEM_R(addr);\
    print_asm(str(name) str(SUFFIX) " %%%s,%s", ModR_M_asm, REG_NAME(m.reg));\
}

#define TEMP_I2RM(name, size) \
if (m.mod == 3) {\
    dest = REG(m.R_M);\
    src = instr_fetch(eip + 2, size);\
    if (size < DATA_BYTE)\
        src = (src << (DATA_BYTE*8 - 8)) >> (DATA_BYTE*8-8);\
    len += 2;\
    print_asm(str(name) str(SUFFIX) " $0x%x, %%%s", src, REG_NAME(m.R_M));\
} else {\
    len += read_ModR_M(eip + 1, &addr);\
    src = instr_fetch(eip + len, size);\
    len += size;\
    dest = MEM_R(addr);\
    print_asm(str(name) str(SUFFIX) " $0x%x,%s", src, ModR_M_asm);\
}

#define TEMP_RESULT2RM(result) \
if (addr == 0) REG(m.R_M) = result;\
else MEM_W(addr, result)

#define TEMP_VALUES \
DATA_TYPE src, dest, result;\
int len = 1

#define TEMP_VALUES_S \
DATA_TYPE_S src, dest, result;\
int len = 1

#define TEMP_MOD_RM \
ModR_M m;\
m.val = instr_fetch(eip + 1, 1);\
swaddr_t addr = 0;

#endif
