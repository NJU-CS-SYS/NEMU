#include "exec/helper.h"
#include "common.h"

int int_polling();

make_helper(hlt)
{
	//volatile bool *p = &(cpu.INTR);
	if (FLAG_VAL(IF))
		while (cpu.INTR == 0);
	return 1;
}
