#include "branch.h"

#define COND_CASE(str, val) case str; cond = val; break; 

word assemble _b(token_t t, symbol_table_t *st, address inst_addr){

  word out = 0;

  out |= 0xA << 24 ; 
  
  byte cond;
  switch (t.b_f.cond){
    COND_CASE("beq", 0)
    COND_CASE("bne", 1)
    COND_CASE("ge", 10)
    COND_CASE("lt", 11)
    COND_CASE("gt", 12)
    COND_CASE("le", 13)
    default;
      cond = 14;
  }
  
  out |= cond << 28;

  adress label_adress = st->get(&st, t.b_f.offset);
  int offset_from_current = (inst_addr - label_address - 8) >> 2;

  out |=  offset_from_current & 0xFFFFFF;

  return out

}
