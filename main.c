#include <stdio.h>
#include <libgccjit.h>

void create_code(gcc_jit_context *ctxt)
{
	/*
	 * ulong loop_test(ulong n)
	 * {
	 * 	ulong sum = 0;
	 * 	for(ulong i = 0; i < n; ++i)
	 * 		sum += i;
	 *
	 * 	return sum;
	 * }
	 */

	/* initialize unsigned long type */
	gcc_jit_type *ulong_type =
		gcc_jit_context_get_type(ctxt, GCC_JIT_TYPE_UNSIGNED_LONG);

	/* function parameter */
	gcc_jit_param *n =
		gcc_jit_context_new_param(ctxt, NULL, ulong_type, "n");

	gcc_jit_param *params[1] = {n};

	gcc_jit_function *func =
		gcc_jit_context_new_function(ctxt, NULL,
				GCC_JIT_FUNCTION_EXPORTED,
				ulong_type,
				"loop_test",
				1, params, 0);
	
	/* build locals */
	gcc_jit_lvalue *sum =
		gcc_jit_function_new_local(func, NULL, ulong_type, "sum");
	gcc_jit_lvalue *i =
		gcc_jit_function_new_local(func, NULL, ulong_type, "i");

	/* prepare looping */
	gcc_jit_block *b_initial =
		gcc_jit_function_new_block(func, "initial");
	gcc_jit_block *b_loop_cond =
		gcc_jit_function_new_block(func, "loop_cond");
	gcc_jit_block *b_loop_body =
		gcc_jit_function_new_block(func, "loop_body");
	gcc_jit_block *b_after_loop =
		gcc_jit_function_new_block(func, "after_loop");

	/* initialize values */
	gcc_jit_block_add_assignment(b_initial, NULL, sum,
			gcc_jit_context_zero(ctxt, ulong_type));
	gcc_jit_block_add_assignment(b_initial, NULL, i,
			gcc_jit_context_zero(ctxt, ulong_type));

	/* connect initial block to condition block */
	gcc_jit_block_end_with_jump(b_initial, NULL, b_loop_cond);

	/* i <= n */
	gcc_jit_block_end_with_conditional(b_loop_cond, NULL,
			gcc_jit_context_new_comparison(ctxt, NULL, GCC_JIT_COMPARISON_LE,
				gcc_jit_lvalue_as_rvalue(i),
				gcc_jit_param_as_rvalue(n)),
			b_loop_body,
			b_after_loop);

	/* sum += i */
	gcc_jit_block_add_assignment_op(b_loop_body, NULL,
			sum, GCC_JIT_BINARY_OP_PLUS,
				gcc_jit_lvalue_as_rvalue(i));

	/* ++i */
	gcc_jit_block_add_assignment_op(b_loop_body, NULL,
			i, GCC_JIT_BINARY_OP_PLUS,
			gcc_jit_context_one(ctxt, ulong_type));

	/* jump back up to condition */
	gcc_jit_block_end_with_jump(b_loop_body, NULL, b_loop_cond);

	/* return sum; */
	gcc_jit_block_end_with_return(b_after_loop, NULL,
			gcc_jit_lvalue_as_rvalue(sum));
}

int main(){
	gcc_jit_context *ctxt = NULL;
	gcc_jit_result *result = NULL;

	ctxt = gcc_jit_context_acquire();
	if(!ctxt){
		fprintf(stderr, "NULL ctxt");
		goto error;
	}

	/* turn on optimizations */
	gcc_jit_context_set_int_option(ctxt, GCC_JIT_INT_OPTION_OPTIMIZATION_LEVEL, 2);

	create_code(ctxt);

	result = gcc_jit_context_compile(ctxt);
	if(!result){
		fprintf(stderr, "NULL result");
		goto error;
	}

	typedef unsigned long (*loop_test_t) (unsigned long);
	loop_test_t loop_test = (loop_test_t)gcc_jit_result_get_code(result, "loop_test");
	if(!loop_test){
		fprintf(stderr, "NULL loop_test");
		goto error;
	}

	printf("%lu\n", loop_test( 1000000000));
error:
	gcc_jit_context_release(ctxt);
	gcc_jit_result_release(result);
	return 0;
}
