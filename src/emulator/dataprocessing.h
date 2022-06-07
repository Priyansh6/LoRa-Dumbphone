#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "../utilities.h"

/* 
Usage:
void execute_DP(instruction_t instruction, state_t *s);

Executes instruction based off of opcode and writes to destination register if required
Sets CPSR flags if required
*/
void execute_DP(instruction_t instruction, state_t *s);

#endif
