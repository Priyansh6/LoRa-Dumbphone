#include <stdlib.h>

#include "multiply.h"

#define M t.contents_f.m_f
#define MA t.contents_f.ma_f
#define SET(r, s) result |= (r << s)
#define SET_FILLER result |= 9 << 4

word assemble_M(token_t t) {
  word result = 14 << 28; // Setting condition code to 14
 
  if (t.format == M_F) {
    SET(M.rd, 16);
    SET(M.rs, 8);
    SET_FILLER;
    SET(M.rm, 0);
  } else {
    result |= 1 << 21;
    SET(MA.r, 16);
    SET(MA.r, 12);
    SET(MA.r, 8);
    SET_FILLER;
    SET(MA.r, 0);
  }

  return result;
}
