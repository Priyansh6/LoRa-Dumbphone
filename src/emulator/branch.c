#include <stdint.h>

#include "branch.h"

#define INSB instruction.contents.b

void execute_B(instruction_t instruction, state_t *s) {
  int32_t off = INSB.offset;

  if (off >> 23) {
    off |= 0xFF000000;
  }

  off <<= 2;

  s->registers[PC] += off;
}
