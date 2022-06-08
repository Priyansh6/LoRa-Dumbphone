#include <string.h>
#include <stdlib.h>

#include "dataprocessing.h"
#include "special.h"
#include "tokeniser.h"

#define LSL t.contents_f.lsl_f

word assemble_ANDEQ(token_t t) {
  return 0;
}

word assemble_LSL(token_t t) {
  char *start_line = "mov ";
  char *mid_line = ",lsl ";

  int start_len = strlen(start_line);
  int mid_len = strlen(mid_line);
  int rn_len = strlen(LSL.rn);
  int expr_len = strlen(LSL.expr);
  int total_len = 2 + start_len + 2 * rn_len + mid_len + expr_len;

  char *new_line = malloc(total_len + 1);
  memcpy(new_line, start_line, start_len);
  memcpy(new_line + start_len, LSL.rn, rn_len);
  new_line[start_len + rn_len] = ',';
  new_line[start_len + rn_len + 1] = ' ';
  memcpy(new_line + 2 + start_len + rn_len, LSL.rn, rn_len);
  memcpy(new_line + 2 + start_len + rn_len * 2, mid_line, mid_len);
  memcpy(new_line + total_len - expr_len, LSL.expr, expr_len);
  new_line[total_len] = '\0';

  free(t.free_pointer);

  return assemble_DP(tokenise_line(new_line));
}
