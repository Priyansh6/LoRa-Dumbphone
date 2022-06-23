#include <stdlib.h>

#include "multiply.h"

#define M t.contents_f.m_f
#define MA t.contents_f.ma_f
#define SET(r, s) instruction |= (r << s)
#define SET_FILLER instruction |= 9 << 4

word assemble_M(token_t t) {
  word instruction = 14 << 28; // Setting condition code to 14
 
  if (t.format == M_F) {
    SET_BITS(instruction, M.rd, 16);
    SET_BITS(instruction, M.rs, 8);
    SET_FILLER;
    SET_BITS(instruction, M.rm, 0);
  } else {
    SET_BITS(instruction, 1, 21);
    SET_BITS(instruction, MA.rd, 16);
    SET_BITS(instruction, MA.rn, 12);
    SET_BITS(instruction, MA.rs, 8);
    SET_FILLER;
    SET_BITS(instruction, MA.rm, 0);
  }

  return instruction;
}

