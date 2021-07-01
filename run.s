.set INSTR_ASM_SIZE, 64

.macro insn i
.space INSTR_ASM_SIZE - ((. - START_INSNS) % INSTR_ASM_SIZE)
.global \i
\i :
.endm


top_run:
mov (%[p]), %rax
add $0x08, %[p]
lea (%rip), %rcx
add %ax, %cx
add $9, %rcx
jmp *%rcx

.global START_INSNS
START_INSNS:

.global END
END:
jmp end_run

insn J
shr $0x10, %rax
movsx %eax, %rax
add %rax, %[p]
jmp top_run

insn JZ
shr $0x10, %rax
movzx %ax, %rcx
mov (%[reg],%rcx, 8), %rcx
add $0x0, %rcx
jnz top_run
shr $0x10, %rax
movsx %eax, %rax
add %rax, %[p]
jmp top_run

insn ADD
shr $0x10, %rax
movzx %ax, %rcx
shr $0x10, %rax
movzx %ax, %r8
shr $0x10, %rax
movzx %ax, %rax
mov (%[reg], %r8, 8), %r8
add (%[reg], %rcx, 8), %r8
mov %r8, (%[reg], %rax, 8)
jmp top_run

insn SUB
shr $0x10, %rax
movzx %ax, %rcx
shr $0x10, %rax
movzx %ax, %r8
shr $0x10, %rax
movzx %ax, %rax
mov (%[reg], %r8, 8), %r8
sub (%[reg], %rcx, 8), %r8
mov %r8, (%[reg], %rax, 8)
jmp top_run

insn LDRC
shr $0x10, %rax
movzx %ax, %rcx
shr $0x10, %rax
movsx %eax, %rax
mov %rax, (%[reg], %rcx, 8)
jmp top_run

end_run:
mov 24(%[reg]), %[r]
