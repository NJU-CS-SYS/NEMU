#include "exec/helper.h"
#include "exec/template-start.h"
#include "cpu/segment.h"
#include "cpu/reg.h"
extern int Sreg;

make_helper(concat(stos_, SUFFIX)) // Store String Data
{
	Sreg = ES;
	// TODO AddressSize variation
	MEM_W(REG(R_EDI),REG(R_EAX));
	if (FLAG_VAL(DF)) REG(R_EDI) -= DATA_BYTE;
	else REG(R_EDI) += DATA_BYTE;
	print_asm("stos" str(SUFFIX));
	return 1;
}

#include "exec/template-end.h"
