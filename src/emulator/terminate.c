#include <stdlib.h>

#include "terminate.h"

void execute_T(instruction_t instruction, state_t *s) {
  pprint_state_t(*s); 
  free(s); 
  exit(EXIT_SUCCESS);
}
