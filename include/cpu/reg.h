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
/* the Control Register 0 */
typedef union CR0 {
	struct {
		uint32_t protect_enable      : 1;
		uint32_t monitor_coprocessor : 1;
		uint32_t emulation           : 1;
		uint32_t task_switched       : 1;
		uint32_t extension_type      : 1;
		uint32_t numeric_error       : 1;
		uint32_t pad0                : 10;
		uint32_t write_protect       : 1; 
		uint32_t pad1                : 1; 
		uint32_t alignment_mask      : 1;
		uint32_t pad2                : 10;
		uint32_t no_write_through    : 1;
		uint32_t cache_disable       : 1;
		uint32_t paging              : 1;
	};
	uint32_t val;
} CR0;

/* the Control Register 3 (physical address of page directory) */
typedef union CR3 {
	struct {
		uint32_t pad0                : 3;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t pad1                : 7;
		uint32_t page_directory_base : 20;
	};
	uint32_t val;
} CR3;

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
		union {
			struct {
				uint16_t es, cs, ss, ds;
			};
			uint16_t seg[4];
		};
		/* control registers */
		union {
			struct {
				CR0 cr0;
				CR0 cr1;
				CR0 cr2;
				CR3 cr3;
			};
			uint32_t cr[4];
		};
		/* globle descriptor table register */
		union {
			struct {
				uint16_t limit;
				uint32_t base;
			};
		}gdtr;
		union {
			struct {
				uint16_t limit;
				uint32_t base;
			};
		}idtr;
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
#define SEG_NAME(index) (segs[index])
#define SEG(index) (cpu.seg[index])
#define FLAG_VAL(index) ((cpu.eflags & (0x1 << index)) == (0x1 << index))
#define FLAG_CHG(index, val) (cpu.eflags = (val) ? (cpu.eflags | (0x1 << index)) : (cpu.eflags & (~(0x1 << index))))

#define PE (cpu.cr0.protect_enable)
extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* crs[];
extern const char* segs[];

#endif
