#include "exec/helper.h"
#include "common.h"
int int_polling();

make_helper(hlt)
{
	while( int_polling() ) {
	}

	test(0, "hlt gets out");
	print_asm("hlt");

	return 1;
}
