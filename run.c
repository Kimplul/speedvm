#include <stdint.h>
#include "instr.h"

uint64_t run(uint64_t *p){
	uint64_t result = 0;
	uint64_t registers[0xffff] = {0};

	// TODO: Optimize
	asm(
		".set INSTR_ASM_SIZE, " INSTR_ASM_SIZE "\n"
		#include "run.inc"

		: [r] "=r" (result)
		: [p] "r" (p), [reg] "r" (registers)
		: "rax", "rcx", "r8", "cc"
	);

	return result;
}
