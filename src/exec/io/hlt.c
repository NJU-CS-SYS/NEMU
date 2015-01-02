#include "exec/helper.h"
#include "common.h"

int int_polling();

make_helper(hlt)
{
	print_asm("hlt");

	return 1;
}
