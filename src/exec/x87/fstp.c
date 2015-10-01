#include "exec/helper.h"
#include "cpu/modrm.h"
#include "nemu.h"
#include "ui/ui.h"
#include "debug.h"

make_helper(fstp_mem_l) {
    ModR_M m;
    m.val = instr_fetch(eip + 1, 1);
    swaddr_t addr;
    Log("eip %x %x", eip , m.val);
    int len = read_ModR_M(eip + 1, &addr);
    Log("Empty implement of fstp, get address %x of length %d, regcode is %x", addr, len, m.reg);
    return 1 + len;
}
