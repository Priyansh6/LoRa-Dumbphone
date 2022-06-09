#ifndef SINGLEDATAPROCESSING_H
#define SINGLEDATAPROCESSING_H

#include "dataprocessing.h"
#include "stack.h"
#include "../utilities.h"

word assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack);

#endif
