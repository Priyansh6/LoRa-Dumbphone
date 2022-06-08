#include "branch.h"
#include <string.h>

#define COND_CASE(str, val) (strcmp(str, t.contents_f.b_f.cond) == 0) {cond = val;} 

word assemble_b(token_t t, symbol_table_t *st, address inst_addr){

  word out = 0;

  out |= 0xA << 24 ; 
  
  byte cond;
 
  if COND_CASE("beq", 0)
  else if COND_CASE("bne", 1)
  else if COND_CASE("ge", 10)
  else if COND_CASE("lt", 11)
  else if COND_CASE("gt", 12)
  else if COND_CASE("le", 13)
  else {
    cond = 14;
  }
  
  out |= cond << 28;

  address label_address = st->get(st, t.contents_f.b_f.offset);
  int offset_from_current = (inst_addr - label_address - 8) >> 2;

  out |=  offset_from_current & 0xFFFFFF;

  return out;

}
