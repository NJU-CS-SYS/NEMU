#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/reg.h"
#include "memory.h"

/* All function defined with 'make_helper' return the length of the instruction. */
#define make_helper(name) int name(swaddr_t eip)

uint32_t instr_fetch(swaddr_t addr, size_t len);

extern char assembly[];
extern char *asm_string;
#define print_asm(...) assert(snprintf(asm_string, 80, __VA_ARGS__) < 80)

#define v_helper(name) \
make_helper(concat(name##_, v)) {\
    return (suffix == 'l' ? concat(name##_, l)(eip) : concat(name##_, w)(eip));\
}

#define all_helper(name) \
make_helper(concat(name##_, b));\
make_helper(concat(name##_, w));\
make_helper(concat(name##_, l));\
make_helper(concat(name##_, v))

#endif
