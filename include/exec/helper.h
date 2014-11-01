#ifndef __EXEC_HELPER_H__
#define __EXEC_HELPER_H__

#include "cpu/reg.h"
#include "memory.h"

/* All function defined with 'make_helper' return the length of the instruction. */
#define make_helper(name) int name(swaddr_t eip)

uint32_t instr_fetch(swaddr_t addr, size_t len);

extern char assembly[];
#define print_asm(...) assert(snprintf(assembly, 40, __VA_ARGS__) < 40)

#define v_helper(name) \
make_helper(concat(name##_, v)) {\
	return (suffix == 'l' ? concat(name##_, l)(eip) : concat(name##_, w)(eip));\
}

#define all_helper(name) \
make_helper(concat(name_, b));\
make_helper(concat(name_, w));\
make_helper(concat(name_, l));\
make_helper(concat(name_, v))

#endif
