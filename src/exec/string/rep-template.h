#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"

make_helper(exec);

make_helper(concat(rep_, SUFFIX)) {
	DATA_TYPE count = REG(R_ECX);
	int len = 0;
	while (count != 0) {
		len = exec(eip+1);
		count --;
	}
	print_asm("rep");
	return len + 1;
}

#include "exec/template-end.h"
