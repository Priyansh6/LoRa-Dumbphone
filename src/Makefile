CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o dataprocessing.o multiply.o singledatatransfer.o branch.o terminate.o utilities.o gpio.o
	gcc emulate.o dataprocessing.o multiply.o singledatatransfer.o branch.o terminate.o utilities.o gpio.o -o emulate

emulate.o: emulate.c

dataprocessing.o: dataprocessing.c

multiply.o: multiply.c

singledatatransfer.o: singledatatransfer.c 

branch.o: branch.c 

terminate.o: terminate.c

utilities.o: utilities.c

gpio.o: gpio.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
