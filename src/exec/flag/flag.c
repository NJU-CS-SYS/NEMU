#define DATA_BYTE 4
#include "exec/helper.h"
#include "exec/template-start.h"
#include "ui/ui.h"

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
make_helper(cli) // Clear Interrupt Flag
{
	FLAG_CHG(IF, 0);
	print_asm("cli");
	nemu_state = TEST_INT;
	return 1;
}
#include "exec/template-end.h"
#undef DATA_BYTE
