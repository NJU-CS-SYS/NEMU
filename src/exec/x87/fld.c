#include "exec/helper.h"
#include "nemu.h"
#include "ui/ui.h"
#include "debug.h"

make_helper(fldz) {
    uint32_t code = instr_fetch(eip, 1);
    Log("Empty function fldz with code %x", code);
    return 2;
}
