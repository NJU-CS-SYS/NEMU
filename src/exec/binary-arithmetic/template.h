#ifndef __TEMPLATE__
#define __TEMPLATE__
#include "exec/helper.h"

#define _TEMP_SUB_I(src, dest, result)\
    do {\
        if (sizeof(src) == 1 && sizeof(dest) > 1) {\
            result = dest - (DATA_TYPE_S)src;\
        } else {\
            result = dest - src;\
        }\
    }while(0)

#define TEMP_FLAG(src, dest, result)\
    do {\
        FLAG_CHG(OF,OVERFLOW(src, dest, result));\
        FLAG_CHG(SF, MSB(result));\
        FLAG_CHG(ZF, result==0);\
        FLAG_CHG(AF, ADJUST(src, dest));\
        FLAG_CHG(PF, PARITY(result));\
        FLAG_CHG(CF, (DATA_TYPE)result < (DATA_TYPE)dest);\
    }while(0)

#define AND_FLAG(src, dest, result)\
    do {\
        FLAG_CHG(OF,0);\
        FLAG_CHG(SF, MSB(result));\
        FLAG_CHG(ZF, result==0);\
        FLAG_CHG(PF, PARITY(result));\
        FLAG_CHG(CF, 0);\
    }while(0)

#define TEMP_ADD_I(src, dest, result)\
do{\
    result = dest + (DATA_TYPE_S)src;\
    TEMP_FLAG(src, dest, result);\
}while(0)

#define TEMP_SUB_I(src, dest, result) \
do{\
_TEMP_SUB_I(src, dest, result);\
TEMP_FLAG(((~src)+1), dest, result);\
}while(0)

#define TEMP_R2RM(name) \
do{\
    src = REG(m.reg);\
    if (m.mod == 3) {\
        dest = REG(m.R_M);\
        len++;\
        print_asm(str(name) str(SUFFIX) " %%%s, %%%s", REG_NAME(m.reg), REG_NAME(m.R_M));\
    } else {\
        len += read_ModR_M(eip + 1, &addr);\
        dest = MEM_R(addr);\
        print_asm(str(name) str(SUFFIX) " %%%s, %s", REG_NAME(m.reg), ModR_M_asm);\
    }\
}while(0)

#define TEMP_RM2R(name) \
do{\
    src = REG(m.reg);\
    if (m.mod == 3) {\
        dest = REG(m.R_M);\
        len++;\
        print_asm(str(name) str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));\
    } else {\
        len += read_ModR_M(eip + 1, &addr);\
        dest = MEM_R(addr);\
        print_asm(str(name) str(SUFFIX) " %%%s,%s", ModR_M_asm, REG_NAME(m.reg));\
    }\
}while(0)

#define TEMP_I2RM(name, size) \
do{\
    if (m.mod == 3) {\
        dest = REG(m.R_M);\
        src = instr_fetch(eip + 2, size);\
        if (size < DATA_BYTE)\
            src = (src << (DATA_BYTE*8 - 8)) >> (DATA_BYTE*8-8);\
        len += (2 + size);\
        print_asm(str(name) str(SUFFIX) " $0x%x, %%%s", src, REG_NAME(m.R_M));\
    } else {\
        len += read_ModR_M(eip + 1, &addr);\
        src = instr_fetch(eip + len, size);\
        len += size;\
        dest = MEM_R(addr);\
        print_asm(str(name) str(SUFFIX) " $0x%x,%s", src, ModR_M_asm);\
    }\
}while(0)

#define TEMP_RESULT2RM(result) \
do{\
    if (addr == 0) REG(m.R_M) = result;\
    else MEM_W(addr, result);\
}while(0)

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
