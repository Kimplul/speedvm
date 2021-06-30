exec: run.o main.o
	$(CC) -g $^ -o $@

main.c: instr.h

%.o: %.c
	$(CC) -c -g $^ -o $@

instr.h: run.c
	@echo "Updating instrs"
	$(CC) -c $^ -o /tmp/instr.o
	objdump -d /tmp/instr.o \
	| sed 's/^0\+/0x/g' \
	| awk -f load_addrs.awk > $@

.PHONY: clean
clean:
	rm exec *.o
