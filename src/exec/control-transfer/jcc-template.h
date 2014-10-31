#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"

make_helper(concat(je_, SUFFIX)) {
	int32_t disp = instr_fetch(eip + 1, DATA_BYTE);
	//Log("zf = %d", FLAG_VAL(ZF));
	if (FLAG_VAL(ZF)) cpu.eip += disp; /* sign extended */
	if (DATA_BYTE == 2) cpu.eip &= 0x0000ffff;
	print_asm("je %x", eip + disp + DATA_BYTE + 1);
	return DATA_BYTE + 1;
}

make_helper(concat(jmp_rel_, SUFFIX)) {
	int32_t imm = instr_fetch(eip + 1, DATA_BYTE);
	cpu.eip += imm;
	print_asm("jmp %x", eip + DATA_BYTE + 1 + imm);
	return 1 + DATA_BYTE;
}

make_helper(concat(jbe_rel_,SUFFIX)) {
	int32_t imm = instr_fetch(eip + 1, DATA_BYTE);
	eip += imm;
	if (DATA_BYTE == 2) eip &= 0x0000ffff;
	if (FLAG_VAL(CF) || FLAG_VAL(ZF)) {
		cpu.eip += eip;
	}
	print_asm("jbe %x", eip + DATA_BYTE + 1);
	return 1 + DATA_BYTE;
}

make_helper(concat(jle_rel_,SUFFIX)) {
	int32_t imm = instr_fetch(eip + 1, DATA_BYTE);
	eip += imm;
	if (DATA_BYTE == 2) eip &= 0x0000ffff;
	if ((FLAG_VAL(OF) != FLAG_VAL(SF)) || FLAG_VAL(ZF)) {
		cpu.eip += eip;
	}
	print_asm("jle %x", eip + DATA_BYTE + 1);
	return 1 + DATA_BYTE;
}
#include "exec/template-end.h"
