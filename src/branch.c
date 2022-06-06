#include <stdint.h>

#include "branch.h"

void execute_B(instruction_t instruction, state_t *s) {
  int32_t off = instruction.contents.b.offset;

  if (off >> 23) {
    off |= 0xFF000000;
  }

  off <<= 2;

  s->registers[PC] += off;
}
