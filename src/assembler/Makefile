CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: dataprocessing.o multiply.o singledatatransfer.o branch.o special.o symboltable.o tokeniser.o binaryfilewriter.o

dataprocessing.o: dataprocessing.c

multiply.o: multiply.c

singledatatransfer.o: singledatatransfer.c 

branch.o: branch.c 

special.o: special.c

symboltable.o: symboltable.c

tokeniser.o: tokeniser.c

binaryfilewriter.o: binaryfilewriter.c

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
