#include <stdint.h>

uint64_t run(uint64_t *p){
	uint64_t result = 0;
	uint64_t registers[0xffff] = {0};

	// TODO: Optimize
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
		"movsx %%eax, %%rax\n"
		"add %%rax, %[p]\n"
		"jmp top_run\n"

		"JZ:\n"
		"shr $0x10, %%rax\n" // ?, JZ
		"movzx %%ax, %%rcx\n"
		"mov (%[reg],%%rcx, 8), %%rcx\n"
		"add $0x0, %%rcx\n"
		"jnz top_run\n"
		"shr $0x10, %%rax\n"
		"movsx %%eax, %%rax\n"
		"add %%rax, %[p]\n"
		"jmp top_run\n"

		"ADD:\n"
		"shr $0x10, %%rax\n" // ?, ADD
		"movzx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"movzx %%ax, %%r8\n"
		"shr $0x10, %%rax\n"
		"movzx %%ax, %%rax\n"
		"mov (%[reg], %%r8, 8), %%r8\n"
		"add (%[reg], %%rcx, 8), %%r8\n"
		"mov %%r8, (%[reg], %%rax, 8)\n"
		"jmp top_run\n"

		"SUB:\n"
		"shr $0x10, %%rax\n" // ?, SUB
		"movzx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"movzx %%ax, %%r8\n"
		"shr $0x10, %%rax\n"
		"movzx %%ax, %%rax\n"
		"mov (%[reg], %%r8, 8), %%r8\n"
		"sub (%[reg], %%rcx, 8), %%r8\n"
		"mov %%r8, (%[reg], %%rax, 8)\n"
		"jmp top_run\n"

		"LDRC:\n"
		"shr $0x10, %%rax\n" // ?, LDRC
		"movzx %%ax, %%rcx\n"
		"shr $0x10, %%rax\n"
		"movsx %%eax, %%rax\n"
		"mov %%rax, (%[reg], %%rcx, 8)\n"
		"jmp top_run\n"

		"end_run:\n"
		"mov 24(%[reg]), %[r]\n"

		: [r] "=r" (result)
		: [p] "r" (p), [reg] "r" (registers)
		: "rax", "rcx", "r8"
	);

	return result;
}
