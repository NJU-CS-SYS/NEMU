#include "exec/helper.h"
#include "common.h"

int int_polling();

make_helper(hlt)
{
	if (FLAG_VAL(IF)) {
		while ( int_polling() );
	}
	test(0,"aaa");
	print_asm("hlt");

	return 1;
}
