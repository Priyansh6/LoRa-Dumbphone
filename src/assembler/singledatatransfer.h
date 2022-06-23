#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include "stack.h"
#include "../utilities.h"

/*
USAGE 
assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack);

Use the fields in the token struct to populate a word with the correspondng object code file
This function is also responsible to add a constant to the stack that would be added at the end of
the second pass.
*/
word assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack);

#endif
