#include <stdlib.h>

#include "multiply.h"

#define M token.contents_f.m_f
#define MA token.contents_f.ma_f
#define SET(r, s) result |= (r << s)
#define SET_FILLER result |= 9 << 4
#define CONVERT(r) strtoul(r + 1, NULL, 10)

word assemble_M(token t) {
  word result = 14 << 28; // Setting condition code to 14
 
  if (t.format == M_F) {
    SET(CONVERT(M.rd), 16);
    SET(CONVERT(M.rs), 8);
    SET_FILLER;
    SET(CONVERT(M.rm), 0);
  } else {
    result |= 1 << 21;
    SET(CONVERT(MA.rd), 16);
    SET(CONVERT(MA.rn), 12);
    SET(CONVERT(MA.rs), 8);
    SET_FILLER;
    SET(CONVERT(MA.rm), 0);
  }

  return result;
}
