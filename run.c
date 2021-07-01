#include <stdint.h>

uint64_t run(uint64_t *p){
	uint64_t result = 0;
	uint64_t registers[0xffff] = {0};

	// TODO: Optimize
	asm(
		#include "run.inc"

		: [r] "=r" (result)
		: [p] "r" (p), [reg] "r" (registers)
		: "rax", "rcx", "r8", "cc"
	);

	return result;
}
