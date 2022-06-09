#include <string.h>
#include <stdlib.h>

#include "dataprocessing.h"
#include "special.h"
#include "tokeniser.h"

#define LSL t.contents_f.lsl_f
#define MOV new_t.contents_f.dp_mov_f

word assemble_ANDEQ(token_t t) {
  return 0;
}

word assemble_LSL(token_t t) {
  token_t new_t;
  new_t.format = DP_MOV_F;
  MOV.rd = LSL.rn;
  MOV.operand2 = (shift_t) { .values_oper_t.sh_reg = 
                           { .rm = LSL.rn, .shifted_vals_t.immediate = LSL.expr } 
                           };

  return assemble_DP(new_t);
}
