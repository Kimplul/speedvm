#include <stdint.h>

#define OPCODE(x) ((uint16_t)(x))

#define RSLOT1(x) ((uint16_t)((x) >> 0x10))
#define RSLOT2(x) ((uint16_t)((x) >> 0x20))
#define RSLOT3(x) ((uint16_t)((x) >> 0x30))

#define VSLOT1(x) ((int32_t)((x) >> 0x10))
#define VSLOT2(x) ((int32_t)((x) >> 0x20))

uint64_t run(uint64_t *p){
	int64_t result = 0;
	int64_t registers[0xffff] = {0};
	--p;

top:
	p++;
	asm goto (
			"START_INSNS:\n"
			"lea 5(%%rip), %%rax\n"
			"add %[p], %%rax\n"
			"jmp *%%rax\n"
			: /* no output */
			: [p] "g" ((uint64_t)OPCODE(*p))
			: "rax", "cc"
			: LDRC, ADD, SUB, JZ, J, END
	    );

END:
	asm("END:\n");
	goto bottom;

LDRC:
	asm("LDRC:\n");
	registers[RSLOT1(*p)] = VSLOT2(*p);
	goto top;

ADD:
	asm("ADD:\n");
	registers[RSLOT3(*p)] = registers[RSLOT2(*p)] + registers[RSLOT1(*p)];
	goto top;

SUB:
	asm("SUB:\n");
	registers[RSLOT3(*p)] = registers[RSLOT2(*p)] - registers[RSLOT1(*p)];
	goto top;

JZ:
	asm("JZ:\n");
	p += (registers[RSLOT1(*p)] == 0 ? VSLOT2(*p) : 0) >> 0x3;
	goto top;

J:
	asm("J:\n");
	p += VSLOT1(*p) >> 0x3;
	goto top;

bottom:

	result = registers[3];

	return result;
}
