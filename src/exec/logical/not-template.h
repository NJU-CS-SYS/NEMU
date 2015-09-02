#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(concat(not_rm_, SUFFIX)) {
    int len = 1;

    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    test(m.reg == 2, "wrong dispatch");
    
    DATA_TYPE buf;
    if (m.mod == 3) {
        buf = REG(m.R_M);
        REG(m.R_M) = ~buf;
        len ++;
        print_asm("not" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
    } else {
        swaddr_t addr;
        len += read_ModR_M(eip + 1, &addr);
        buf = MEM_R(addr);
        MEM_W(addr, ~buf);
        print_asm("not" str(SUFFIX) " %s", ModR_M_asm);
    }

    return len;
}

#include "exec/template-end.h"
