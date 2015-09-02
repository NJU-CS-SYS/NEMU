#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "../template.h"

#define TEMP_XOR_FLAG(src, dest, result) \
do {\
    FLAG_CHG(CF, 0);\
    FLAG_CHG(OF, 0);\
    FLAG_CHG(SF, MSB(result));\
    FLAG_CHG(ZF, 0 == result);\
    FLAG_CHG(PF, PARITY(result));\
} while (0)

make_helper(concat(xor_i2r_, SUFFIX)) {
    TEMP_VALUES;
    src = instr_fetch(eip + 1, DATA_BYTE);
    dest = REG(R_EAX);
    result = dest ^ src;
    REG(R_EAX) = result;
    print_asm("xor" str(SUFFIX) " $0x%x,%%%s", src, REG_NAME(R_EAX));
    len = 1 + DATA_BYTE;
    TEMP_XOR_FLAG(src, dest, result);
    return len;
}

make_helper(concat(xor_i2rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    test(m.reg == 6, "wrong dispatching");
    TEMP_I2RM(xor, DATA_BYTE);
    
    result = dest ^ src;

    TEMP_XOR_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    
    return len;
}

make_helper(concat(xor_r2rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_R2RM(xor);
    result = dest ^ src;
    TEMP_XOR_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}

make_helper(concat(xor_rm2r_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_RM2R(xor);
    result = dest ^ src;
    TEMP_XOR_FLAG(dest, src, result);
    REG(m.reg) = result; 
    return len;
}

make_helper(concat(xor_i82rm_, SUFFIX)) {
    TEMP_VALUES;
    TEMP_MOD_RM;
    TEMP_I2RM(xor, 1);
    result = dest ^ src;
    TEMP_XOR_FLAG(src, dest, result);
    TEMP_RESULT2RM(result);
    return len;
}
#include "exec/template-end.h"
