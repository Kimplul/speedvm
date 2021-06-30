#include <stdio.h>
#include "instr.h"
#include "run.h"

void compile_ldrc(uint64_t *p, uint16_t reg, int32_t val){
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

void compile_j(uint64_t *p, int32_t offset){
	*p = J
		| ((uint64_t)offset << 16)
		;
}

void compile_jz(uint64_t *p, uint16_t reg, int32_t offset){
	*p = JZ
		| ((uint64_t)reg << 16)
		| ((uint64_t)offset << 32)
		;
}

void compile_end(uint64_t *p){
	*p = END;
}

int main(){
	uint64_t p[16] = {0};

	compile_ldrc(&p[0], 0, 1);
	compile_ldrc(&p[1], 1, 1000000000);
	compile_ldrc(&p[2], 2, 0);
	compile_ldrc(&p[3], 3, 0);

	compile_sub(&p[4], 3, 1, 4);
	compile_jz(&p[5], 4, 24);

	compile_add(&p[6], 2, 3, 3);
	compile_add(&p[7], 1, 2, 2);

	compile_j(&p[8], -40);

	compile_end(&p[9]);

	printf("%lli\n", run(p)); 

	return 0;
}
