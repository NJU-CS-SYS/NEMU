#ifndef __CPU_REG_H__
#define __CPU_REG_H__

#include "common.h"

/* TODO: Re-organize the 'CPU_state' structure to match
 * the GPR encoding scheme in i386 instruction format.
 * For example, if we access reg_w(R_BX) we will get the 'bx' register;
 * if we access reg_b(R_CH), we will get the 'ch' register.
 * Hint: Use 'union'.
 * For more details about the GPR encoding scheme, see i386 manual.
 */

typedef union {
	union {
		uint32_t _32;
		uint16_t _16;
		uint8_t _8[2];
	} gpr[8];

	/* Do NOT change the order of the GPRs' definitions. 
	 * They match the register encoding scheme used in i386 instruction format.
	 * See i386 manual for more details.
	 */

	struct {
		uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
		swaddr_t eip;
		uint32_t eflags;
		/*
		 * 0x 00 00 00 01, CF, Carry Flag
		 * 0x 00 00 00 04, PF, Parity Flag
		 * 0x 00 00 00 10, AF, Auxiliary Carry
		 * 0x 00 00 00 40, ZF, Zero Flag
		 * 0x 00 00 00 80, SF, Sign Flag
		 * 0x 00 00 01 00, TF, Trap Flag
		 * 0x 00 00 02 00, IF, Interrupt Enable
		 * 0x 00 00 04 00, DF, Direction Flag
		 * 0x 00 00 08 00, OF, Overflow Flag
		 * 0x 00 00 30 00, IO PL, I/O Privilege Level
		 * 0x 00 00 40 00, NT, Nested Task Flag
		 * 0x 00 01 00 00, RF, Resume Flag
		 * 0x 00 02 00 00, VM, Virtual 8086 Mode
		 */
		/* segment registers */
		uint16_t cs, ds, ss, es, fs, gs;
		/* control registers */
		uint32_t cr0;
		/* globle descriptor table register */
		uint32_t gdtr;
	};
} CPU_state;

extern CPU_state cpu;

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { CF, CONSERVE_1_1, PF, CONSERVE_0_2, AF, CONSERV_0_3, ZF, SF, TF, IF, DF, OF, IOPL, IOPL_CON, NT, CONSERV_O_4, RF, VM};

#define reg_l(index) (cpu.gpr[index]._32)
#define reg_w(index) (cpu.gpr[index]._16)
#define reg_b(index) (cpu.gpr[index & 0x3]._8[index >> 2])
#define FLAG_VAL(index) ((cpu.eflags & (0x1 << index)) == (0x1 << index))
#define FLAG_CHG(index, val) (cpu.eflags = (val) ? (cpu.eflags | (0x1 << index)) : (cpu.eflags & (~(0x1 << index))))

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
