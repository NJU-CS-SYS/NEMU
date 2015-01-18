#include "exec/helper.h"
#include "cpu/reg.h"
#include "ui/ui.h"

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
	printf("word=%04x, dword=%08x", word, dword);

	print_asm("cwtl");

	nemu_state = TEST_INT;

	return 1;
}
