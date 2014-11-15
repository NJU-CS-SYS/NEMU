#include "exec/helper.h"
#include "cpu/modrm.h"

#include "all-instr.h"
#include "group/group.h"
make_helper(nop);

typedef int (*helper_fun)(swaddr_t);

/* TODO: Add more instructions!!! */

helper_fun opcode_table [256] = {
/* 0x00 */	add_r2rm_b, add_r2rm_v, add_rm2r_b, add_rm2r_v, 
/* 0x04 */	add_i2r_b, add_i2r_v, inv, inv, 
/* 0x08 */  or_r2rm_b, or_r2rm_v, or_rm2r_b, or_rm2r_v, 
/* 0x0c */	or_i2r_b, or_i2r_v, inv, opcode_2, 
/* 0x10 */	adc_r2rm_b, adc_r2rm_v, adc_rm2r_b, adc_rm2r_v, 
/* 0x14 */	adc_i2r_b, adc_i2r_v, inv, inv, 
/* 0x18 */	sbb_r2rm_b, sbb_r2rm_v, sbb_rm2r_b, sbb_rm2r_v, 
/* 0x1c */	sbb_i2r_b, sbb_i2r_v, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	and_i2r_b, and_i2r_v, inv, inv,
/* 0x28 */	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v, 
/* 0x2c */	sub_i2r_b, sub_i2r_v, sub_i2r_v, inv, 
/* 0x30 */	xor_r2rm_b, xor_r2rm_v, xor_rm2r_b, xor_rm2r_v, 
/* 0x34 */	xor_i2r_b, xor_i2r_v, inv, inv,
/* 0x38 */	cmp_r2rm_b, cmp_r2rm_v, cmp_rm2r_b, cmp_rm2r_v, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inc_r_v, inc_r_v, inc_r_v, inc_r_v,  
/* 0x44 */  inc_r_v, inc_r_v, inc_r_v, inc_r_v,
/* 0x48 */	dec_r_v, dec_r_v, dec_r_v, dec_r_v, 
/* 0x4c */	dec_r_v, dec_r_v, dec_r_v, dec_r_v,  
/* 0x50 */	push_v, push_v, push_v, push_v, 
/* 0x54 */	push_v, push_v, push_v, push_v,
/* 0x58 */	pop_stack2r_v, pop_stack2r_v, pop_stack2r_v, pop_stack2r_v, 
/* 0x5c */	pop_stack2r_v, pop_stack2r_v, pop_stack2r_v, pop_stack2r_v, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, data_size, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	jo_rel_b, jno_rel_b, jb_rel_b, jae_rel_b,
/* 0x74 */	je_b, jne_rel_b, jbe_rel_b, ja_rel_b,
/* 0x78 */	js_rel_b, jns_rel_b, jp_rel_b, jnp_rel_b, 
/* 0x7c */	jl_rel_b, jge_rel_b, jle_rel_b, jg_rel_b, 
/* 0x80 */	group_80, group_81, nemu_trap, group_83, 
/* 0x84 */	test_r_rm_b, test_r_rm_v, inv, inv, 
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea_v, inv, group_8f, 
/* 0x90 */	nop, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	group_c0, group_c1, inv, ret_near_v,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, leave_l, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	group_d0, group_d1, group_d2, group_d3,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, jcxz_rel_v,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_rel_v, jmp_rel_v, inv, jmp_rel_b,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, group_f6, group_f7,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, group_fe, group_ff
};

helper_fun opcode_table_2 [256] = {
/* 0x00 */	inv, inv, inv, inv,
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv,
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv,	
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	jo_rel_v, jno_rel_v, jb_rel_v, jae_rel_v,
/* 0x84 */	je_v, jne_rel_v, jbe_rel_v, ja_rel_v,
/* 0x88 */	inv, jns_rel_v, jp_rel_v, jnp_rel_v,
/* 0x8c */	jl_rel_v, jge_rel_v, jle_rel_v, jg_rel_v, 
/* 0x90 */	set_o, set_no, set_c, set_ae,
/* 0x94 */	set_e, set_ne_nz, set_be, set_a,
/* 0x98 */	set_s, set_ns, set_pe, set_po, 
/* 0x9c */	set_l, set_ge, set_le, set_g, 
/* 0xa0 */	inv, inv, inv, inv,
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	inv, inv, inv, inv,
/* 0xb4 */	inv, inv, movz_b2_v, movz_w2_l,
/* 0xb8 */	inv, inv, inv, inv,
/* 0xbc */	inv, inv, movs_b2r_v, movs_w2r_l,
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

helper_fun group_table_80[8] = {
	add_i2rm_b, or_i2rm_b, adc_i2rm_b, sbb_i2rm_b,
	and_i2rm_b, sub_i2rm_b, xor_i2rm_b, cmp_i8_rm_b
};

helper_fun group_table_81[8] = {
	add_i2rm_v, or_i2rm_v, adc_i2rm_v, sbb_i2rm_v,
	and_i2rm_v, sub_i2rm_v, xor_i2rm_v, cmp_i2rm_v
};

helper_fun group_table_83[8] = {
	add_i82rm_v, or_i82rm_v, adc_i82rm_v, sbb_i82rm_v,
	and_i82rm_v, sub_i82rm_v, xor_i82rm_v, cmp_i8_rm_v
};

helper_fun group_table_c0[8] = {
	inv, inv, inv, inv,
	sal_i8_b, shr_i8_b, inv, sar_i8_b
};

helper_fun group_table_c1[8] = {
	inv, inv, inv, inv,
	sal_i8_v, shr_i8_v, inv, sar_i8_v 
};

helper_fun group_table_d0[8] = {
	inv, inv, inv, inv,
	sal_12rm_b, shr_12rm_b, inv, sar_12rm_b
};

helper_fun group_table_d1[8] = {
	inv, inv, inv, inv,
	sal_12rm_v, shr_12rm_v, inv, sar_12rm_v
};

helper_fun group_table_d2[8] = {
	inv, inv, inv, inv,
	sal_r2rm_b, shr_r2rm_b, inv, sar_r2rm_b
};

helper_fun group_table_d3[8] = {
	inv, inv, inv, inv,
	sal_r2rm_v, shr_r2rm_v, inv, sar_r2rm_v
};

helper_fun group_table_8f[8] = {
	pop_stack2m_v, inv, inv, inv,
	inv, inv, inv, inv
};

helper_fun group_table_f6[8] = {
	inv, inv, not_rm_b, neg_rm_b,
	inv, inv, inv, inv
};

helper_fun group_table_f7[8] = {
	inv, inv, not_rm_v, neg_rm_v,
	inv, inv, inv, inv
};

helper_fun group_table_fe[8] = {
	inc_rm_b, dec_rm_b, call_rm_l, inv,
	inv, inv, inv, inv
};

helper_fun group_table_ff[8] = {
	inc_rm_v, dec_rm_v, call_rm_l, inv,
	inv, inv, inv, inv
};

make_helper(exec) {
	return opcode_table[instr_fetch(eip, 1)](eip);
}

make_helper(nop) {
	print_asm("nop");
	return 1;
}
