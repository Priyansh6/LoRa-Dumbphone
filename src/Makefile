CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o utilities.o 
	$(MAKE) -C emulator
	gcc emulate.o emulator/dataprocessing.o emulator/multiply.o emulator/singledatatransfer.o emulator/branch.o emulator/terminate.o utilities.o emulator/gpio.o -o emulate

emulate.o: emulate.c

utilities.o: utilities.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
