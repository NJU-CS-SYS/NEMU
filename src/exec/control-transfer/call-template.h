#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

swaddr_t read_func_name(swaddr_t addr, swaddr_t *value);

#define DEBUG(name) \
Log("esp in " str(name) " %x", cpu.esp)

make_helper(concat(call_rel_, SUFFIX)) {
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

	swaddr_t func_name = read_func_name(eip + 1 + DATA_BYTE, NULL);
	/*
	print_asm("call" " %x <%s+0x%x>",
					eip + 1 + DATA_BYTE,
					(char*)func_name,
					eip + 1 + DATA_BYTE - func_name
					);
	*/
	print_asm("call" " %x <%s>", eip + 1 + DATA_BYTE, (char*)func_name);
	return 1 + DATA_BYTE;
}

make_helper(concat(call_rm_, SUFFIX)) {
	DATA_TYPE dest;
	int len = 1;
	ModR_M m;
	swaddr_t addr = 0;
	m.val = instr_fetch(eip + 1, 1);
	test(m.reg == 2, "wrong dispatching");
	if (m.mod == 3) {
		dest = REG(m.R_M);
		len++; // for modrm byte
		print_asm("call %%%s", REG_NAME(m.R_M));
	} else {
		len += read_ModR_M(eip + 1, &addr); // for modrm, sib, and addr
		dest = MEM_R(addr);
		print_asm("call *0x%s", ModR_M_asm);
	}
	if (DATA_BYTE == 2) {
		PUSH((DATA_TYPE)((eip & 0x0000ffff) + len -1)); // 1 for ret to return 1
		cpu.eip = dest & 0x0000ffff;
	} else {
		cpu.eip = dest;
	}
	cpu.eip -= len;
	return len;
}

/* leave */
make_helper(concat(leave_, SUFFIX)) {
	cpu.esp = cpu.ebp;
	POP(concat(reg_, SUFFIX)(R_EBP));
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
	RET_COMMON;
	print_asm("ret");
	return 1;
}

#undef RET_COMMON

#include "exec/template-end.h"
