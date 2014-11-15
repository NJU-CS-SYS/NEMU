#define DATA_BYTE 4
#include "exec/helper.h"
#include "exec/template-start.h"

make_helper(cld) {
	FLAG_CHG(DF, 0);
	print_asm("cld");
	return 1;
}
make_helper(std) {
	FLAG_CHG(DF, 1);
	print_asm("std");
	return 1;
}
#include "exec/template-end.h"
#undef DATA_BYTE
