#include "exec/helper.h"
#include "cpu/modrm.h"
#include "../template.h"

#define DATA_BYTE 4
#include "exec/template-start.h"
make_helper(lea_l) {
	swaddr_t addr;
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	test(m.mod != 3, "not a ea");
	int len = read_ModR_M(eip + 1, &addr);
	print_asm("leal %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	REG(m.reg) = addr;
	return len + 1;
}
#include "exec/template-end.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "exec/template-start.h"
make_helper(lea_w) {
	swaddr_t addr;
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	test(m.mod != 3, "not a ea");
	int len = read_ModR_M(eip + 1, &addr);
	print_asm("leaw %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	REG(m.reg) = addr;
	return len + 1;
}
#include "exec/template-end.h"
#undef DATA_BYTE

extern char suffix;
v_helper(lea)
