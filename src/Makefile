CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o utilities.o
	$(MAKE) -C assembler
	gcc assemble.o utilities.o assembler/dataprocessing.o assembler/linkedlist.o assembler/multiply.o assembler/singledatatransfer.o assembler/branch.o assembler/special.o assembler/symboltable.o assembler/tokeniser.o assembler/binaryfilewriter.o -o assemble

assemble.o: assemble.c

emulate: emulate.o utilities.o 
	$(MAKE) -C emulator
	gcc emulate.o utilities.o emulator/dataprocessing.o emulator/multiply.o emulator/singledatatransfer.o emulator/branch.o emulator/terminate.o emulator/gpio.o -o emulate

emulate.o: emulate.c

utilities.o: utilities.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
