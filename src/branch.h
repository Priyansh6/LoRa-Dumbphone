#ifndef BRANCH_H
#define BRANCH_H

#include "utilities.h"

/*
Usage:
execute_B(instruction_t instruction, state_t *s)

Executes a branch instruction. Increments the PC register by the offset.
*/
void execute_B(instruction_t instruction, state_t *s);

#endif
