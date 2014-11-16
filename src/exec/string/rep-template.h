#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "stdio.h"

make_helper(exec);

make_helper(concat(rep_, SUFFIX)) {
	int len = 0;
	print_asm("rep ");
	asm_string += 4;
	while (REG(R_ECX) != 0) {
		len = exec(eip+1);
		REG(R_ECX) --;
	}
	asm_string -= 4;
	return len + 1;
}

#include "exec/template-end.h"
