#ifndef DATAPROCESSING_H
#define DATAPROCESSING_H

#include "utilities.h"

void execute_DP(instruction_t, state_t*);
void execute_M(instruction_t, state_t*);
void execute_SDT(instruction_t, state_t*);
void execute_B(instruction_t, state_t*);

#endif
