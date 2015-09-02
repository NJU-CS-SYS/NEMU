#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

#define SBB_FLAG(src, dest, result) do {\
    DATA_TYPE add_src = ~src + 1;\
    if (MSB(src) == MSB(dest)) { /* here src and dest is the same sign actually */ \
        FLAG_CHG(OF,0);\
    } else {\
        FLAG_CHG(OF,MSB(dest) != MSB(result));\
    }\
    FLAG_CHG(SF, MSB(result));\
    FLAG_CHG(ZF, result==0);\
    FLAG_CHG(AF, ADJUST(add_src, dest));\
    FLAG_CHG(PF, PARITY(result));\
    FLAG_CHG(CF, (DATA_TYPE)src + FLAG_VAL(CF) > (DATA_TYPE)dest);\
}while(0)

#define SBB(src, dest, result) do {\
        result = dest - (src + FLAG_VAL(CF));\
        SBB_FLAG(src, dest, result);\
    } while (0)

make_helper(concat(sbb_i2r_, SUFFIX)) {
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
    DATA_TYPE dest = REG(R_EAX);
    DATA_TYPE result = dest;
    SBB(imm, dest, result);
    REG(R_EAX) = result;

    print_asm("sbb" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(R_EAX));
    return DATA_BYTE + 1;
}

make_helper(concat(sbb_i2rm_, SUFFIX)) {
    TEMP_VALUES_S;
    TEMP_MOD_RM;
    TEMP_I2RM(sbb, DATA_BYTE);
    SBB(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(sbb_i82rm_, SUFFIX)) {
    TEMP_VALUES_S;
    TEMP_MOD_RM;
    TEMP_I2RM(sbb, 1);
    int shift_len = (DATA_BYTE - 1) * 8;
    src = (src << shift_len) >> shift_len;
    SBB(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(sbb_r2rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_R2RM(sbb);
    SBB(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(sbb_rm2r_, SUFFIX)) {
    TEMP_VALUES_S;
    TEMP_MOD_RM;
    TEMP_RM2R(sbb);
    SBB(dest, src, result);
    REG(m.reg) = result;
    return len;
}
#include "exec/template-end.h"
