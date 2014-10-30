#include "exec/helper.h"
#include "dispatch.h"
#include "cpu/modrm.h"
make_helper(cmp_i8_rm_v);
make_helper(reg_83_v) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	switch (m.reg) {
		case 7: return cmp_i8_rm_v(eip);
	}
	test(0, "Invalid modrm reg domain with this opcode");
	return 0;
}
