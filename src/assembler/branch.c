#include "branch.h"
#include <string.h>
#include <stdio.h>

#define COND_CASE(str, val) (strcmp(str, t.contents_f.b_f.cond) == 0) {cond = val;} 

word assemble_B(token_t t, symbol_table_t *st, address inst_addr){
  word out = 0;

  out |= 0xA << 24 ; 
  
  byte cond;
 
  if COND_CASE("beq", 0)
  else if COND_CASE("bne", 1)
  else if COND_CASE("bge", 10)
  else if COND_CASE("blt", 11)
  else if COND_CASE("bgt", 12)
  else if COND_CASE("ble", 13)
  else {
    cond = 14;
  }
  
  out |= cond << 28;

  address label_address = st->get(st, t.contents_f.b_f.offset);

  int offset_from_current = (label_address - inst_addr - 8) >> 2;

  out |=  offset_from_current & 0xFFFFFF;

  return out;
}

