#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define Rotate_Left(buf, cnt)\
    do {                                   \
        uint8_t temp;                      \
        while (cnt != 0)                   \
        {                                  \
            temp = MSB(buf);               \
            buf = (buf << 1) | temp;       \
            cnt --;                        \
        }                                  \
    } while(0)

make_helper(concat(rol_i82rm_, SUFFIX))
{
    int8_t imm8;
    DATA_TYPE buf;

    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    assert(m.opcode == 0);

    if (m.mod == 3)
    {
        imm8 = instr_fetch(eip + 2, 1);
        buf = REG(m.R_M);

        Rotate_Left(buf, imm8);
        REG(m.R_M) = buf;
        
        print_asm("rol" str(SUFFIX) " $%x,%%%s", imm8, REG_NAME(m.R_M));
        return 3;
    }
    else
    {
        swaddr_t addr;
        int len = read_ModR_M(eip + 1, &addr);
        buf = MEM_R(addr);
        imm8 = instr_fetch(eip + len, 1);

        Rotate_Left(buf, imm8);
        MEM_W(addr, buf);

        print_asm("rol" str(SUFFIX) " $%x,%s", imm8, ModR_M_asm);
        return 1 + len + 1;
    }
}
#include "exec/template-end.h"
