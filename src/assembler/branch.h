#ifndef BRANCH_H
#define BRANCH_H

#include "../utilities.h"
#include "symboltable.h"

/*
USAGE 
assemble_B(token_t t);

This function is tasked with populating a word with the format specified
by a branch instruction.
*/
word assemble_B(token_t t, symbol_table_t *st, address inst_addr);

#endif
