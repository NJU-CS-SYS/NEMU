#include "exec/helper.h"
#include "cpu/reg.h"

make_helper(cwtl)
/* 
 * Sign-extended word to dword
 */
{
	int16_t word;
	int32_t dword;

	word = reg_w(R_AX);
	dword = word;        // Sign-extended
	reg_l(R_EAX) = dword;

	print_asm("cwtl");

	return 1;
}
