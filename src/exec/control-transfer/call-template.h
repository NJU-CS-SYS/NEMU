#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/segment.h"

#define DEBUG(name) \
Log("esp in " str(name) " %x", cpu.esp)

make_helper(concat(call_rel_, SUFFIX)) {
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
     if (DATA_BYTE == 2) {
        PUSH((DATA_TYPE)((eip & 0x0000ffff) + DATA_BYTE));
        eip = (eip + imm) & 0x0000ffff;
        cpu.eip = eip;
    } else {
        PUSH(eip + DATA_BYTE);
        eip += imm;
        cpu.eip = eip;
     }

    print_asm("call" " %x", eip + 1 + DATA_BYTE);
    return 1 + DATA_BYTE;
}

make_helper(concat(call_rm_, SUFFIX)) {
    DATA_TYPE dest;
    int len = 1;
    ModR_M m;
    swaddr_t addr = 0;
    m.val = instr_fetch(eip + 1, 1);
    test(m.reg == 2, "wrong dispatching");

    if (m.mod == 3) {
        dest = REG(m.R_M);
        len++; // for modrm byte
        print_asm("call %%%s", REG_NAME(m.R_M));
    } else {
        len += read_ModR_M(eip + 1, &addr); // for modrm, sib, and addr
        dest = MEM_R(addr);
        print_asm("call %s", ModR_M_asm);
    }

    if (DATA_BYTE == 2) {
        PUSH((DATA_TYPE)((eip & 0x0000ffff) + len -1)); // 1 for ret to return 1
        cpu.eip = dest & 0x0000ffff;
    } else {
        PUSH(eip + len - 1); // 1 for ret to return 1
        cpu.eip = dest;
    }
    cpu.eip -= len;
    return len;
}

/* leave */
make_helper(concat(leave_, SUFFIX)) {
    cpu.esp = cpu.ebp;
    POP(concat(reg_, SUFFIX)(R_EBP));
    print_asm("leave");
    return 1;
}

make_helper(concat(ret_near_, SUFFIX)) {
#if DATA_BYTE == 2
    DATA_TYPE ip;
    POP(ip);
    cpu.eip = ip & 0x0000ffff;
#else
    POP(cpu.eip);
#endif
    print_asm("ret");
    return 1;
}

#if DATA_BYTE != 1
make_helper(concat(ret_imm16_, SUFFIX)) {
    uint16_t imm16 = instr_fetch(eip + 1, 2);
#if DATA_BYTE == 2
    DATA_TYPE pc;
    POP(pc);
    cpu.eip = pc & 0x0000ffffU;
#elif DATA_BYTE == 4
    POP(cpu.eip);
#endif
    cpu.esp -= imm16;

    print_asm("ret");
    return 1;
}
#endif

make_helper(concat(iret_, SUFFIX))
{
    /* TODO This instruction is not implement fully! */
    POP(cpu.eip);
    POP(cpu.cs);
    POP(cpu.eflags);

    /* TODO This amend of eip is expected the intr is caused by
     * instruction `int' with 2 bytes in total. This may not be
     * always correct, so pay attention to this code when you
     * run into fault
     */

    /* UPDATE: raise_intr can be triggered by any INTR, so the
     * above presume is incorrect.
     */

    cpu.eip --;
    //Log("eip has sub 1 for iret return len");
    print_asm("iret" str(SUFFIX));
    return 1;
}

#include "exec/template-end.h"
