#include "exec/helper.h"
#include "cpu/modrm.h"
#include "exec/template-start.h"
#include "stdio.h"

make_helper(exec);

make_helper(concat(rep_, SUFFIX)) {
	int len = 0;
	printf("rep ");
	fflush(stdout);
	while (REG(R_ECX) != 0) {
		len = exec(eip+1);
		REG(R_ECX) --;
	}
	return len + 1;
}

#include "exec/template-end.h"
