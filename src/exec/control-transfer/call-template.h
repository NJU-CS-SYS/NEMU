#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(call_rel_, SUFFIX)) {
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	if (DATA_BYTE == 2) {
		PUSH((DATA_TYPE)(eip & 0x0000ffff));
		eip = (eip + imm) & 0x0000ffff;
		cpu.eip = eip;
	} else {
		PUSH(eip);
		eip += imm;
		cpu.eip = eip;
	}
	print_asm("call" " $0x%x", eip + 1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

/* leave */
make_helper(concat(leave_, SUFFIX)) {
	cpu.esp = cpu.ebp;
	POP(concat(reg_, SUFFIX)(R_ESP));
	print_asm("leave");
	return 1;
}

#define RET_COMMON \
if (DATA_BYTE == 2) {\
	DATA_TYPE ip;\
	POP(ip);\
	cpu.eip = ip & 0x0000ffff;\
} else {\
	POP(cpu.eip);\
}

#ifndef __RET_C3__
#define __RET_C3__
make_helper(ret_near) {
	log(cpu.eip);
	RET_COMMON;
	log(cpu.eip);
	test(0, "...");
	print_asm("ret");
	return 1;
}
#endif

#undef RET_COMMON

#include "exec/template-end.h"
