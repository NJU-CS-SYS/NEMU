#include "exec/helper.h"
#include "io/port-io.h"
#include "exec/template-start.h"

make_helper(concat(in_i2a_, SUFFIX))
{
	int8_t ioaddr = instr_fetch(eip + 1, 1);
	REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);
	
	print_asm("in $%#x", ioaddr);

	return 2;
}

make_helper(concat(in_d2a_, SUFFIX))
{
	int8_t ioaddr = reg_l(R_DX);
	REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);
	
	print_asm("in %%%s,%s", REG_NAME(R_EAX), "dx");

	return 1;
}

#include "exec/template-end.h"
