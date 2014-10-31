#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
extern char suffix;

#define JCC_LEN_DEF int len = DATA_BYTE + (suffix == 'b' ? 1 : 2)

#define JCC_COMMON_REL(condition, name) \
int32_t imm = instr_fetch(eip + 1, DATA_BYTE);\
eip += imm;\
if (suffix == 'w') eip &= 0x0000ffff;\
if ((condition)) cpu.eip = eip;\
print_asm(str(name) " %x", eip + len);\
return len

make_helper(concat(je_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(ZF), je);
}

make_helper(concat(jmp_rel_, SUFFIX)) {
	int len = 1 + DATA_BYTE;
	JCC_COMMON_REL(1, jmp);
}

make_helper(concat(jbe_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL((FLAG_VAL(CF) || FLAG_VAL(ZF)), jbe);
}

make_helper(concat(jle_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(((FLAG_VAL(OF) != FLAG_VAL(SF)) || FLAG_VAL(ZF)), jle);
}

#include "exec/template-end.h"
