CFLAGS := -Wall -O2 -g

exec: run.o main.o
	$(CC) $(CFLAGS) $^ -o $@

main.c: instr.h
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

instr.h: run.o
	@echo "Updating instrs"
	readelf -s $< \
	| awk '/INSTR/{print $$2, $$8}' \
	| sort \
	| sed 's/^0\+/0x/g' \
	| awk -f load_addrs.awk > $@

.PHONY: clean
clean:
	rm exec instr.h *.o
