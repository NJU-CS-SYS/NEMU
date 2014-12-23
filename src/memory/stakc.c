#include "common.h"
#include "memory.h"
#include "cpu/reg.h"

void push(uint32_t data)
{
	cpu.esp -= 4;
	Log("new esp %x", cpu.esp);
	swaddr_write(cpu.esp, 4, data);
}
