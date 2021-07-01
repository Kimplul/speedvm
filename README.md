# speedvm
Proof of concept VM instruction set

# Concept
Most scripting languages use a virtual machine (VM) with instruction sets, somewhat similar to real CPUs.
Decoding these instructions is a fairly slow process, and is usually done by jump tables generated by the compiler.
What this demo tries to show is how you can (to an extent) skip the decoding phase by using the opcode as a jumpt address directly.

Because compilers can't realistically do this automatically, some assembly is required. How much is up to the implementer, and in
this repo I've demonstrated two approaches.

# Structure

+ `main` branch: Hand-written assembly VM implementation with fixed-width instruction space. The assembly can be found in `run.s`, and it's
included as inline assembly in `run.c`. By fixed-width instruction space, I mean that each instruction implementation gets a certain amount of bytes
to implement its functionality. This could come in handy if you want to be able to run pre-compiled code on different architectures, as you can hard-code
opcodes. See `instr.h` for an example.

+ `cc` branch: Mixed assembly and C with variable-width instruction space. The code can be found in `run.c`, with most computation happening in C
and only the opcode decoding written in assembly. Variable-width instruction space does not allow for inter-arch pre-compiled code execution, but significantly
cuts down on code footprint. The binary produces by GCC on my system was slightly slower than the hand-written assembly version, but the added benefit of
being able to use a high-level language is notable.

Note that you can use either fixed-width or variable-width with either C or assembly, I just chose these two combinations for demonstration purposes.

# Implemented instructions
+ `END` - signify end of program, exit out of VM
+ `J` - jump fixed amount unconditionally
+ `JZ` - jump fixed amount if zero
+ `ADD` - add two registers together and place result into a third register
+ `SUB` - subtract two registers together and place result into a third register
+ `LDRC` - load constant value into register

For more info look into `main.c`.

# Test application
The test case I chose to use was a simple loop that sums the first billion integers. The speedVM 'assembly' can be found in `main.c`, and some popular
scripting language equivalents in `loop.*`. Here's a table of how long each file took to run:

| Language     | Time (s) |
|--------------|----------|
| speedVM, C   | 9.25     |
| speedVM, ASM | 8.36     |
| AWK          | 43.14    |
| Perl         | 42.35    |
| Lua          | 18.61    |

Note that I do not claim this test is in any way fair, as speedVM doesn't have a language front-end and the loop I manually wrote is very tight.

# Compiling
`make`

# Running
`exec`

# General notes, thoughts and ramblings
This is not supposed to be the fastes possible VM, I'm sure you could come up with more efficient assembly or even a faster instruction set. For example,
`ADD` and `SUB` use three operands, but only two are strictly necessary in this case. This leads to extra memory operations, and thus to slower execution.
Also, if you wanted EXTREME looping speed from your scripting language, a JIT compiler would probably be the best bet. JIT compilers, however, have larger
overhead and in situations where there is very little code reentrance, could end up being slower than a more naive approach, such as this one.
