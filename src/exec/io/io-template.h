#include "exec/helper.h"
#include "io/port-io.h"
#include "ui/ui.h"
#include "exec/template-start.h"
make_helper(concat(in_i2a_, SUFFIX))
{
	int16_t ioaddr = instr_fetch(eip + 1, 1);
	REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);
	
	print_asm("in $%#x", ioaddr);

	return 2;
}
make_helper(concat(in_d2a_, SUFFIX))
{
	int16_t ioaddr = reg_l(R_DX);
	REG(R_EAX) = pio_read(ioaddr, DATA_BYTE);
	
	Log("eax %x, dx %x", REG(R_EAX), ioaddr);

	print_asm("in %s,%%%s", "(%dx)", REG_NAME(R_EAX));

	return 1;
}
make_helper(concat(out_i2a_, SUFFIX))
{
	int16_t ioaddr = instr_fetch(eip + 1, 1);
	pio_write( ioaddr, DATA_BYTE, REG(R_EAX) );
	
	print_asm("out %#x", ioaddr);
	return 2;
}
make_helper(concat(out_d2a_, SUFFIX))
{
	int16_t ioaddr = reg_w(R_DX);
	Log("ioaddr %x, data %x %c", ioaddr, REG(R_EAX), REG(R_EAX));
	pio_write( ioaddr, DATA_BYTE, REG(R_EAX) );

	print_asm("out %%%s,%s", REG_NAME(R_EAX), "(%dx)");
	return 1;
}

#include "exec/template-end.h"
