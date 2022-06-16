#ifndef SINGLEDATAPROCESSING_H
#define SINGLEDATAPROCESSING_H

#include "dataprocessing.h"
#include "stack.h"
#include "../utilities.h"

/*
USAGE 
assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack);

Use the feilds in the token struct to populate a word with the corresponidng object code file
This function is also responiable to add a constant to the stack that would be added at the edn o
the second pass.
*/
word assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack);

#endif
