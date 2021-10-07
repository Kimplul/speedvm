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

#define INSTR_SWITCH(p)\
	switch(OPCODE(*(++p))){\
		case END: goto bottom; \
		case LDRC: goto LDRC_LABEL; \
		case ADD: goto ADD_LABEL; \
		case SUB: goto SUB_LABEL; \
		case JZ: goto JZ_LABEL; \
		case J: goto J_LABEL;\
	}

#define INSTR_START(i) i##_LABEL:
#define INSTR_END(p) INSTR_SWITCH(p)

uint64_t run(int64_t *p)
{
	int64_t result = 0;
	int64_t registers[0xffff] = {0};
	--p;

	INSTR_SWITCH(p);

	INSTR_START(LDRC);
	registers[RSLOT1(*p)] = VSLOT2(*p);
	INSTR_END(p);

	INSTR_START(ADD);
	registers[RSLOT3(*p)] = registers[RSLOT2(*p)] + registers[RSLOT1(*p)];
	INSTR_END(p);

	INSTR_START(SUB);
	registers[RSLOT3(*p)] = registers[RSLOT2(*p)] - registers[RSLOT1(*p)];
	INSTR_END(p);

	INSTR_START(JZ);
	p = ADDPD(p, (registers[RSLOT1(*p)] == 0 ? VSLOT2(*p) : 0));
	INSTR_END(p);

	INSTR_START(J);
	p = ADDPD(p, VSLOT1(*p));
	INSTR_END(p);
bottom:

	result = registers[3];

	return result;
}
