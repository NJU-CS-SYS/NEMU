#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"

make_helper(concat(and_i82rm_, SUFFIX)) {
    TEMP_VALUES_S;
    TEMP_MOD_RM;
    TEMP_I2RM(and, 1);
    int shift_len = (DATA_BYTE - 1) * 8;
    src = (src << shift_len) >> shift_len;
    result = dest & src;
    TEMP_AND_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(and_i2r_, SUFFIX)) {
    TEMP_VALUES;
    src = instr_fetch(eip + 1, DATA_BYTE);
    len += DATA_BYTE;
    dest = REG(R_EAX);
    result = dest & src;
    TEMP_AND_FLAG(src, dest, result);
    REG(R_EAX) = result;
    print_asm("and" str(SUFFIX) " $%#x,%s", src, REG_NAME(R_EAX));
    return len;
}

make_helper(concat(and_i2rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    test(m.reg == 4, "wrong dispatch");
    TEMP_I2RM(and, DATA_BYTE);
    result = src & dest;
    TEMP_AND_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(and_r2rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_R2RM(and);
    result = src & dest;
    TEMP_AND_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    test(len > 1, "too short len");
    return len;
}

make_helper(concat(and_rm2r_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_RM2R(and);
    result = src & dest;
    TEMP_AND_FLAG(src, dest, result);
    REG(m.reg) = result;
    return len;
}
#include "exec/template-end.h"
