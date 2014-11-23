#include "trap.h"
char str[] = "abc";
int main() {
	nemu_assert(str[0] == 'a');
	nemu_assert(str[1] == 'b');
	nemu_assert(str[2] == 'c');
	nemu_assert(str[3] == '\0');
	HIT_GOOD_TRAP;
	return 0;
}
