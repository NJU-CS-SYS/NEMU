#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define SET(x, name) \
	do {\
		int len = 2;\
		ModR_M m;\
		m.val = instr_fetch(eip + 2, 1);\
		if (m.mod == 3) {\
			REG(m.R_M) = (x);\
			print_asm(str(name) " %%%s", REG_NAME(m.reg));\
			return len + 1;\
		} else {\
			swaddr_t addr;\
			len += read_ModR_M(eip + 2, &addr);\
			MEM_W(addr, (x));\
			print_asm(str(name) " %s", ModR_M_asm);\
			return len;\
		}\
	} while (0)

make_helper(set_ne_nz) {
	SET(!FLAG_VAL(ZF), setne);
}

make_helper(set_o) {
	SET(FLAG_VAL(OF), seto);
}
make_helper(set_no) {
	SET(!FLAG_VAL(OF), setno);
}
make_helper(set_c) {
	SET(FLAG_VAL(CF), setc);
}
make_helper(set_ae) {
	SET(!FLAG_VAL(CF), setae);
}
make_helper(set_e) {
	SET(FLAG_VAL(ZF), setae);
}
make_helper(set_be) {
	SET((FLAG_VAL(CF) || FLAG_VAL(ZF)), setbe);
}
make_helper(set_a) {
	SET(!(FLAG_VAL(CF) || FLAG_VAL(ZF)), seta);
}
make_helper(set_s) {
	SET(FLAG_VAL(SF), sets);
}
make_helper(set_ns) {
	SET(!FLAG_VAL(SF), setns);
}
make_helper(set_pe) {
	SET(FLAG_VAL(PF), setpe);
}
make_helper(set_po) {
	SET(!FLAG_VAL(PF), setpo);
}
make_helper(set_l) {
	SET(FLAG_VAL(SF) != FLAG_VAL(OF), setl);
}
make_helper(set_ge) {
	SET(FLAG_VAL(SF) == FLAG_VAL(OF), setge);
}
make_helper(set_le) {
	SET(FLAG_VAL(ZF) && (FLAG_VAL(SF) != FLAG_VAL(OF)), setle);
}
make_helper(set_g) {
	SET(!FLAG_VAL(ZF) && (FLAG_VAL(SF) == FLAG_VAL(OF)), setg);
}

#include "exec/template-end.h"
