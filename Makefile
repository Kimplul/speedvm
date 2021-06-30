CFLAGS := -Wall -O2

exec: run.o main.o
	$(CC) $(CFLAGS) -g $^ -o $@

%.o: %.c instr.h
	$(CC) $(CFLAGS) -c -g $< -o $@

instr.h: run.c
	@echo "Updating instrs"
	$(CC) $(CFLAGS) -c -g $^ -o /tmp/instr.o
	objdump -d /tmp/instr.o \
	| sed 's/^0\+/0x/g' \
	| awk -f load_addrs.awk > $@

.PHONY: clean
clean:
	rm -- exec instr.h *.o
