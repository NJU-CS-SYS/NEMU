#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "cpu/reg.h"

/* this head file deals with instructions
 * such as push and pop which operate the stack,
 * and have the alike format when in 16 bits and
 * 32 bits situation
 * NOTE: not all push and pop can aply this template,
 * remember to implement them!
 */

/* 50 +rb PUSH r16  PUSH register word
 * 50 +rw PUSH r32  PUSH register dword
 */

#undef MEM_W
#if DATA_BYTE == 2
  #define MEM_W(x, y) swaddr_write(x, DATA_BYTE, y)
#else
  #define MEM_W(x, y) swaddr_write(x, 4, y)
#endif

make_helper(concat(push_, SUFFIX)) {
    int reg_code = instr_fetch(eip, 1) & 0x7;
#if DATA_BYTE == 2
    MEM_W( reg_l(R_ESP) - 2, REG(reg_code) );
    reg_l(R_ESP) -= 2;
#else
    MEM_W( reg_l(R_ESP) - 4, REG(reg_code) );
    reg_l(R_ESP) -= 4;
#endif

    print_asm("push" str(SUFFIX) " %%%s", REG_NAME(reg_code));
    return 1;
}

make_helper(concat(push_i2s_, SUFFIX))
{
    DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);

    cpu.esp -= 4;
    MEM_W(cpu.esp, imm);

    print_asm("push" str(SUFFIX) " $%#x", imm);
    return 1 + DATA_BYTE;
}
make_helper(concat(push_m2s_, SUFFIX))
{
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    test ((m.mod != 3) && (m.reg == 6), "Bad ModR/M Byte!");
    swaddr_t addr;
    int len = read_ModR_M(eip + 1, &addr);
    DATA_TYPE src = MEM_R(addr);

    cpu.esp -= 4;
    MEM_W(cpu.esp, src);

    print_asm("push" str(SUFFIX) " %s", ModR_M_asm);
    return 1 + len;
}
make_helper(concat(pop_stack2m_, SUFFIX)) {
    ModR_M m;
    int len;
    DATA_TYPE buf = MEM_R(REG(R_ESP));; // to receive the top of stack
    m.val = instr_fetch(eip + 1, 1);
    test(m.reg == 0, "wrong dispatch");
    if (m.mod == 3) {
        REG(m.R_M) = buf;
        len = 2;
        print_asm("pop" str(SUFFIX) " %%%s", REG_NAME(m.R_M));
    } else {
        swaddr_t addr;
        len = 1 + read_ModR_M(eip + 1, &addr);
        MEM_W(addr, buf);
        print_asm("pop" str(SUFFIX) " %s", REG_NAME(m.R_M));
    }
    cpu.esp += DATA_BYTE;
    return len;
}

make_helper(concat(pop_stack2r_, SUFFIX))
{
    int regcode = instr_fetch(eip, 1) & 0x7;
    REG(regcode) = MEM_R(REG(R_ESP));
    cpu.esp += DATA_BYTE;
    print_asm("pop" str(SUFFIX) " %%%s", REG_NAME(regcode));
    return 1;
}
make_helper(concat(pusha_, SUFFIX))
{
    const DATA_TYPE temp_esp = REG(R_ESP);
    PUSH(REG(R_EAX));
    PUSH(REG(R_ECX));
    PUSH(REG(R_EDX));
    PUSH(REG(R_EBX));
    PUSH(temp_esp);
    PUSH(REG(R_EBP));
    PUSH(REG(R_ESI));
    PUSH(REG(R_EDI));

    test(temp_esp == REG(R_ESP) + (DATA_BYTE << 3),
            "temp_esp %x, new_esp %x", temp_esp, REG(R_ESP));
    print_asm("pusha");
    return 1;
}
make_helper(concat(popa_, SUFFIX))
{
    POP(REG(R_EDI));
    POP(REG(R_ESI));
    POP(REG(R_EBP));
    POP(REG(R_EBX));
    POP(REG(R_EBX));
    POP(REG(R_EDX));
    POP(REG(R_ECX));
    POP(REG(R_EAX));

    print_asm("popa" str(SUFFIX));
    return 1;
}
#include "exec/template-end.h"
