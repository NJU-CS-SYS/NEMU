#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/modrm.h"
#include "../template.h"
extern char suffix;

#define JCC_LEN_DEF int len = DATA_BYTE + (DATA_BYTE == 1 ? 1 : 2);

#if DATA_BYTE == 1
#define OFFSET 1
#else
#define OFFSET 2
#endif

#define JCC_COMMON_REL(condition, name) \
DATA_TYPE_S imm = instr_fetch(eip + OFFSET, DATA_BYTE);\
eip += imm;\
if (suffix == 'w') eip &= 0x0000ffff;\
if ((condition)) cpu.eip = eip;\
print_asm(str(name) " %x", eip + len);\
return len

#define make_jcc(name, cond)\
make_helper(concat(name##_rel_, SUFFIX)) {\
	JCC_LEN_DEF;\
	JCC_COMMON_REL((cond), name);\
}

make_helper(concat(jmp_rel_, SUFFIX)) {
	int len = 1 + DATA_BYTE;
	DATA_TYPE_S imm = instr_fetch(eip + 1, DATA_BYTE);
	eip += imm;
	if (suffix == 'w') eip &= 0x0000ffff;
	cpu.eip = eip;
	print_asm("jmp %x", eip + len);
	return len;
}

make_helper(concat(jmp_rm_, SUFFIX)) {
	int len = 1;
	DATA_TYPE src;
	ModR_M m;
	GET_RM(src, len);

	cpu.eip += MEM_R(src);

	if (m.mod == 3) {
		print_asm("jmp *%%%s", REG_NAME(m.R_M));
		Log("addr = %x dest = %x", REG(m.R_M), MEM_R(REG(m.R_M)));
	} else {
		print_asm("jmp *%s", ModR_M_asm);
	}
	return len;
}

make_helper(concat(je_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(ZF), je);
}
make_helper(concat(jne_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(ZF) == 0, jne);
}

make_helper(concat(jb_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(CF)==1, jb);
}
make_helper(concat(jbe_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL((FLAG_VAL(CF) || FLAG_VAL(ZF)), jbe);
}

make_helper(concat(jcxz_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(reg_w(R_CX)==0, jcxz);
}
make_helper(concat(jecxz_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(reg_l(R_ECX)==0, jecxz);
}

make_helper(concat(jl_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL((FLAG_VAL(OF) != FLAG_VAL(SF)), jl);
}
make_helper(concat(jle_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(((FLAG_VAL(OF) != FLAG_VAL(SF)) || FLAG_VAL(ZF)), jle);
}


make_helper(concat(jg_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(((!FLAG_VAL(ZF))&&(FLAG_VAL(SF)==FLAG_VAL(OF))), jg);
}
make_helper(concat(jge_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(OF)==FLAG_VAL(SF), jge);
}

make_helper(concat(ja_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL((FLAG_VAL(CF) == 0) && (FLAG_VAL(ZF) == 0), ja);
}
make_helper(concat(jae_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(CF) == 0, jae);
}

make_helper(concat(jna_rel_, SUFFIX)) {
	JCC_LEN_DEF;
	JCC_COMMON_REL(FLAG_VAL(CF) || FLAG_VAL(ZF), jna);
}
make_jcc(jnae, FLAG_VAL(CF))
make_jcc(jnb, FLAG_VAL(CF==0))
make_jcc(jnbe, (FLAG_VAL(CF)==0)&&(FLAG_VAL(ZF)==0))
make_jcc(jng, FLAG_VAL(ZF) || (FLAG_VAL(SF) != FLAG_VAL(OF)))
make_jcc(jnge, FLAG_VAL(SF) != FLAG_VAL(OF))
make_jcc(jnl, FLAG_VAL(SF)==FLAG_VAL(OF))
make_jcc(jnel, (FLAG_VAL(SF)==FLAG_VAL(OF)) && (FLAG_VAL(ZF)==0))
make_jcc(jno, !FLAG_VAL(OF))
make_jcc(jnp, !FLAG_VAL(PF))
make_jcc(jns, !FLAG_VAL(SF))
make_jcc(jnz, !FLAG_VAL(ZF))
make_jcc(jo, FLAG_VAL(OF))
make_jcc(jp, FLAG_VAL(PF))
make_jcc(js, FLAG_VAL(SF))

#undef OFFSET
#include "exec/template-end.h"
