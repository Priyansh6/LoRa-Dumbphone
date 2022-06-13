CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean assembler emulator

all: assembler emulator assemble emulate 

assemble: assemble.o utilities.o assembler
	gcc assemble.o utilities.o assembler/stack.o assembler/dataprocessing.o assembler/multiply.o assembler/singledatatransfer.o assembler/branch.o assembler/special.o assembler/symboltable.o assembler/tokeniser.o assembler/binaryfilewriter.o -o assemble

assembler:
	$(MAKE) -C assembler

assemble.o: assemble.c

emulate: emulate.o utilities.o emulator
	gcc emulate.o utilities.o emulator/dataprocessing.o emulator/multiply.o emulator/singledatatransfer.o emulator/branch.o emulator/terminate.o emulator/gpio.o -o emulate

emulator:
	$(MAKE) -C emulator

emulate.o: emulate.c

utilities.o: utilities.c utilities.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
