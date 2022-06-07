#ifndef MULTIPLY_H
#define MULTIPLY_H

#include "../utilities.h"

/*
Usage:
execute_M(instruction_t instruction, state_t *s);
Executes a multiply instruction. Modifies the CPSR register depending on whether the multiply overflows. 
*/
void execute_M(instruction_t instruction, state_t *s);

#endif
