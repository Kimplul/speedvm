CFLAGS := -Wall -O2 -g

exec: run.o main.o
	$(CC) $(CFLAGS) $^ -o $@

main.c: run.h instr.h
run.c: run.inc run.h instr.h
%.o: %.c
	$(CC) -c -g $< -o $@

run.inc: run.s
	@echo "Updating ASM"
	sed -n 's/\\/\\\\/g; s/^\(.\+\)/"\1\\n"/; s/%\([^[]\)/%%\1/g; p' $< > $@

.PHONY: clean
clean:
	rm exec run.inc *.o
