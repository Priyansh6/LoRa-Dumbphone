#include <stdint.h>
#include <stdbool.h>

#include "utilities.h"

void execute_B(instruction_t type, state_t* state) {
  int32_t off = type.contents.b.offset;

  bool sign = (off >> 23);

  if (sign) {
    off |= 0xFF000000;
  }

  off = (off << 2);

  state->registers[15] += off;
}
