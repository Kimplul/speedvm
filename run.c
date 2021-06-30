#include <stdint.h>

uint64_t run(uint64_t *p){
	uint64_t result = 0;
	uint64_t registers[0xffff] = {0};

	asm(
		"top_run:\n"
		"mov (%[p]), %%rax\n"
		"add $0x08, %[p]\n"
		"lea (%%rip), %%rcx\n"
		"add %%ax, %%cx\n"
		"add $9, %%rcx\n"
		"jmp *%%rcx\n"

		"END:\n"
		"jmp end_run\n" // 0, END

		"J:\n"
		"shr $0x10, %%rax\n" // ?, J
		"add %%rax, %[p]\n"
		"jmp top_run\n"

		"JZ:\n"
		"shr $0x10, %%rax\n" // ?, JZ
		"mov %%rax, %%rcx\n"
		"and $0xffff, %%rcx\n"
		"mov (%[reg],%%rcx), %%rcx\n"
		"test $0x0, %%cx\n"
		"jz top_run\n"
		"shr $0x10, %%rax\n"
		"movsx %%eax, %%rax\n"
		"add %%rax, %[p]\n"
		"jmp top_run\n"

		"ADD:\n"
		"shr $0x10, %%rax\n" // ?, ADD
		"movsx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"movsx %%ax, %%r8\n"
		"movsx %%ax, %%rax\n"
		"mov (%[reg], %%rax), %%rax\n"
		"add (%[reg], %%rcx), %%rax\n"
		"mov %%rax, (%[reg], %%r8)\n"
		"jmp top_run\n"

		"SUB:\n"
		"shr $0x10, %%rax\n" // ?, SUB
		"movsx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"movsx %%ax, %%r8\n"
		"movsx %%ax, %%rax\n"
		"mov (%[reg], %%rax), %%rax\n"
		"sub (%[reg], %%rcx), %%rax\n"
		"mov %%rax, (%[reg], %%r8)\n"
		"jmp top_run\n"

		"LDRC:\n"
		"shr $0x10, %%rax\n" // ?, LDRC
		"movsx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"mov %%eax, (%[reg], %%rcx)\n"
		"jmp top_run\n"

		"end_run:\n"
		"mov 0x03(%[reg]), %[r]\n"

		: [r] "=r" (result)
		: [p] "r" (p), [reg] "r" (registers)
		: "rax", "rcx", "r8"
	);

	return result;
}
