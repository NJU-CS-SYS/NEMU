#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

#define DEBUG(name) \
Log("esp in " str(name) " %x", cpu.esp)
make_helper(concat(call_rel_, SUFFIX)) {
	DEBUG(call);
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	if (DATA_BYTE == 2) {
		PUSH((DATA_TYPE)((eip & 0x0000ffff) + DATA_BYTE));
		eip = (eip + imm) & 0x0000ffff;
		cpu.eip = eip;
	} else {
		PUSH(eip + DATA_BYTE);
		eip += imm;
		cpu.eip = eip;
	}
	DEBUG(call);
	print_asm("call" " $0x%x", eip + 1 + DATA_BYTE);
	return 1 + DATA_BYTE;
}

/* leave */
make_helper(concat(leave_, SUFFIX)) {
	DEBUG(leave);
	cpu.esp = cpu.ebp;
	DEBUG(leave after ebp);
	POP(concat(reg_, SUFFIX)(R_EBP));
	DEBUG(leave_after_pop);
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

make_helper(concat(ret_near_, SUFFIX)) {
	DEBUG(ret);
	RET_COMMON;
	DEBUG(after_ret);
	print_asm("ret");
	return 1;
}

#undef RET_COMMON

#include "exec/template-end.h"
