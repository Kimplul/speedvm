#include "instr.h"
#include "run.h"

void compile_ldrc(uint64_t *p, uint16_t reg, uint16_t val){
	*p = LDRC
		| ((uint64_t)reg << 16)
		| ((uint64_t)val << 32)
		;
}

void compile_add(uint64_t *p, uint16_t src0, uint16_t src1, uint16_t dst){
	*p = ADD
		| ((uint64_t)src0 << 16)
		| ((uint64_t)src1 << 32)
		| ((uint64_t)dst << 48)
		;
}

void compile_sub(uint64_t *p, uint16_t src0, uint16_t src1, uint16_t dst){
	*p = SUB
		| ((uint64_t)src0 << 16)
		| ((uint64_t)src1 << 32)
		| ((uint64_t)dst << 48)
		;
}

void compile_jz(uint64_t *p, int32_t offset){
	*p = JZ
		| ((uint64_t)offset << 16)
		;
}

void compile_j(uint64_t *p, uint16_t reg, int32_t offset){
	*p = J
		| ((uint64_t)reg << 16);
		| ((uint64_t)offset << 32)
		;
}

void compile_end(uint64_t *p){
	*p = END;
}

int main(){

	return 0;
}
