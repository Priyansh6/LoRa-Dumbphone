#include "utilities.h"
#include <stdlib.h>

void execute_T(instruction_t instruction, state_t *s) {
  pprint_state_t(*s); 
  exit(EXIT_SUCCESS);
}