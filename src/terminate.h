#ifndef TERMINATE_H
#define TERMINATE_H

#include "utilities.h"

/*
Usage:
execute_T(instruction_t instruction, state_t *s);
This pretty prints the final state of all registers and then performs a system exit.
*/
void execute_T(instruction_t instruction, state_t *s);

#endif
