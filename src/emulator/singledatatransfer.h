#ifndef SINGLEDATATRANSFER_H
#define SINGLEDATATRANSFER_H

#include "../utilities.h"

/*
Usage
execute_SDT(instruction_t instruction, state_t *s);

Executes a Single Data Transfer instruction. The program will first calculate the adress in question from the instruction_t instruction.
Then depending on the L flag in instruction it will either - Load the data stored in memory at the adress calculated into the register rd
                                                           - Store the date stored in the register rd into memory at the location calculated
*/
void execute_SDT(instruction_t instruction, state_t *s);

#endif
