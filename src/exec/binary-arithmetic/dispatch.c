#include "exec/helper.h"
#include "dispatch.h"
#include "cpu/modrm.h"

#define GET_REG ModR_M m; m.val = instr_fetch(eip + 1, 1)
#define FAIL_DISPATCH \
test(0, "Invalid modrm reg domain");\
return 0

make_helper(sub_i2rm_b);
make_helper(cmp_i8_rm_b);
make_helper(reg_80_v) {
	GET_REG;
	switch(m.reg) {
		case 5: return sub_i2rm_b(eip);
		case 7: return cmp_i8_rm_b(eip);
	}
	test(0, "Invalid modrm reg domain");
	return 0;
}

make_helper(sub_i2rm_v);
make_helper(reg_81_v) {
	GET_REG;
	switch(m.reg) {
		case 5: return sub_i2rm_v(eip);
	}
	test(0, "Invalid modrm reg domain");
	return 0;
}

make_helper(cmp_i8_rm_v);
make_helper(reg_83_v) {
	GET_REG;
	switch (m.reg) {
		case 7: return cmp_i8_rm_v(eip);
	}
	FAIL_DISPATCH;
}
