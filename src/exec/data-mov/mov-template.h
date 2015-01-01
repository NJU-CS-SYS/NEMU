#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"
#include "cpu/reg.h"
#include "cpu/segment.h"
void tlb_init();
extern int Sreg;

make_helper(concat(mov_i2r_, SUFFIX)) {
	int reg_code = instr_fetch(eip, 1) & 0x7;
	DATA_TYPE imm = instr_fetch(eip + 1, DATA_BYTE);
	REG(reg_code) = imm;

	print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(reg_code));
	return DATA_BYTE + 1;
}

make_helper(concat(mov_i2rm_, SUFFIX)) {
	ModR_M m;
	DATA_TYPE imm;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		imm = instr_fetch(eip + 1 + 1, DATA_BYTE);
		REG(m.R_M) = imm;
		print_asm("mov" str(SUFFIX) " $0x%x,%%%s", imm, REG_NAME(m.R_M));
		return 1 + DATA_BYTE + 1;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		imm = instr_fetch(eip + 1 + len, DATA_BYTE);
		MEM_W(addr, imm);
		print_asm("mov" str(SUFFIX) " $0x%x,%s", imm, ModR_M_asm);
		return len + DATA_BYTE + 1;
	}
}

make_helper(concat(mov_r2rm_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.R_M) = REG(m.reg);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.reg), REG_NAME(m.R_M));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		MEM_W(addr, REG(m.reg));

		print_asm("mov" str(SUFFIX) " %%%s,%s", REG_NAME(m.reg), ModR_M_asm);
		return len + 1;
	}
}

make_helper(concat(mov_rm2r_, SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	if(m.mod == 3) {
		REG(m.reg) = REG(m.R_M);
		print_asm("mov" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
		return 2;
	}
	else {
		swaddr_t addr;
		int len = read_ModR_M(eip + 1, &addr);
		REG(m.reg) = MEM_R(addr);

		print_asm("mov" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
		return len + 1;
	}
}

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

make_helper(concat(movz_b2_, SUFFIX)) 
{
	uint8_t src;
	int len = 2;
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	if (m.mod == 3) { // reg
		src = reg_b(m.R_M);
		print_asm("movzb" str(SUFFIX) " %%%s,%%%s", regsb[m.R_M], REG_NAME(m.reg));
		len++;
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 2, &addr);
		src = swaddr_read(addr, 1); 
		
		print_asm("movzb" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
    	}
	REG(m.reg) = src;
	REG(m.reg) &= 0x000000ff;
	return len;
}

make_helper(concat(movz_w2_, SUFFIX)) 
{
	uint16_t src;
	int len = 2;
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	if (m.mod == 3) { // reg
		src = REG(m.R_M);
		len++;

		print_asm("movzw" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M),REG_NAME(m.reg));
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 2, &addr);
		src = swaddr_read(addr, 2);
		
		print_asm("movzw" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
  	}
	REG(m.reg) = src;
	REG(m.reg) &= 0x0000ffff;
	return len;
}

make_helper(concat(movs_b2r_, SUFFIX)) {
	DATA_TYPE_S src;
	int len = 2;
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	if (m.mod == 3) { // reg
		src = reg_b(m.R_M);
		len++;

		print_asm("movsb" str(SUFFIX) " %%%s,%%%s", regsb[m.R_M], REG_NAME(m.reg));
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 2, &addr);
		src = MEM_R(addr);
		
		print_asm("movsb" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}
	int shift_len = (DATA_BYTE - 1) * 8;
	src = (src << shift_len) >> shift_len;
	REG(m.reg) = src;
	return len;
}

make_helper(concat(movs_w2r_, SUFFIX)) {
	DATA_TYPE_S src;
	int len = 2;
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	if (m.mod == 3) { // reg
		src = REG(m.R_M);
		len++;

		print_asm("movsw" str(SUFFIX) " %%%s,%%%s", REG_NAME(m.R_M), REG_NAME(m.reg));
	} else {
		swaddr_t addr;
		len += read_ModR_M(eip + 2, &addr);
		src = MEM_R(addr);
		
		print_asm("movsw" str(SUFFIX) " %s,%%%s", ModR_M_asm, REG_NAME(m.reg));
	}
	int shift_len = (DATA_BYTE - 2) * 8;
	src = (src << shift_len) >> shift_len;
	REG(m.reg) = src;
	return len;
}

make_helper(concat(movs_m2m_, SUFFIX)) {
	Sreg = DS;
	DATA_TYPE buf = MEM_R( cpu.esi );
	Sreg = ES;
	MEM_W( cpu.edi, buf );
	print_asm("movs %%ds:%%%s,%%es:%%%s", regsl[R_ESI], regsl[R_EDI]);
	if (FLAG_VAL(DF)) {
		cpu.edi -= DATA_BYTE;
		cpu.esi -= DATA_BYTE;
	} else {
		cpu.edi += DATA_BYTE;
		cpu.esi += DATA_BYTE;
	}
	return 1;
}

#if DATA_BYTE == 4
make_helper(mov_CR2r) 
{
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	REG(m.R_M) = CR(m.reg);
	print_asm("mov %s,%s", CR_NAME(m.reg), REG_NAME(m.R_M));
	return 3;
}

make_helper(mov_r2CR)
{
	tlb_init();
	ModR_M m;
	m.val = instr_fetch(eip + 2, 1);
	CR(m.reg) = REG(m.R_M);
	print_asm("mov %s,%s", REG_NAME(m.R_M), CR_NAME(m.reg));
	return 3;
}

make_helper(mov_r2seg)
{
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	test(m.mod == 3, "wrong");
	SEG(m.reg) = REG(m.R_M);
	print_asm("mov %s,%s", REG_NAME(m.R_M), SEG_NAME(m.reg));
	return 1 + 1;
}
#endif
#include "exec/template-end.h"
