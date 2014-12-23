#include "memory.h"
#include "cpu/reg.h"

void push(uint32_t data)
{
	cpu.esp -= 4;
	swaddr_write(cpu.esp, data, 4);
}
