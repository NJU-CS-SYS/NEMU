#include "exec/helper.h"
#include "common.h"

int int_polling();

make_helper(hlt)
{
	if (FLAG_VAL(IF)) {
		printf("hit");
		while ( int_polling() );
	}
	print_asm("hlt");

	return 1;
}
