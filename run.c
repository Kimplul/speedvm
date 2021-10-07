#include <stdint.h>
#include "instr.h"

#define OPCODE(x) ((uint16_t)(x))

#define RSLOT1(x) ((uint16_t)((x) >> 0x10))
#define RSLOT2(x) ((uint16_t)((x) >> 0x20))
#define RSLOT3(x) ((uint16_t)((x) >> 0x30))

#define VSLOT1(x) ((int32_t)((x) >> 0x10))
#define VSLOT2(x) ((int32_t)((x) >> 0x20))

#define ADDPD(x, y) (int64_t *)(((int8_t *)(x)) + (y))

#define QUOTE(x) #x

#define INSTR(x) x: asm(".global " QUOTE(INSTR_##x) "\n" QUOTE(INSTR_##x)":\n")

uint64_t run(int64_t *p)
{
	int64_t result = 0;
	int64_t registers[0xffff] = {0};
	--p;

top:
	switch(OPCODE(*(++p))){
		case END:
			goto bottom;

		case LDRC:
			registers[RSLOT1(*p)] = VSLOT2(*p);
			goto top;

		case ADD:
			registers[RSLOT3(*p)] = registers[RSLOT2(*p)] + registers[RSLOT1(*p)];
			goto top;

		case SUB:
			registers[RSLOT3(*p)] = registers[RSLOT2(*p)] - registers[RSLOT1(*p)];
			goto top;

		case JZ:
			p = ADDPD(p, (registers[RSLOT1(*p)] == 0 ? VSLOT2(*p) : 0));
			goto top;

		case J:
			p = ADDPD(p, VSLOT1(*p));
			goto top;
	}

bottom:

	result = registers[3];

	return result;
}
