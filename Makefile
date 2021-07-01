CFLAGS := -Wall -O2

exec: run.o main.o
	$(CC) -g $^ -o $@

main.c: instr.h
run.c: run.inc 
%.o: %.c
	$(CC) -c -g $< -o $@

instr.h: run.o
	@echo "Updating instrs"
	objdump -d $< \
	| sed 's/^0\+/0x/g' \
	| awk -f load_addrs.awk > $@

run.inc: run.s
	@echo "Updating ASM"
	sed -n 's/\\/\\\\/g; s/^\(.\+\)/"\1\\n"/; s/%\([^[]\)/%%\1/g; p' $< > $@

.PHONY: clean
clean:
	rm exec instr.h run.inc *.o
