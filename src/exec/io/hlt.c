#include "exec/helper.h"
#include "common.h"

int int_polling();

make_helper(hlt)
{
	if (FLAG_VAL(IF))
		while (cpu.INTR == 0);	
	print_asm("hlt");

	return 1;
}
