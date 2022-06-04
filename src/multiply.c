#include <stdio.h>
#include <stdint.h>

#include "utilities.h"

void execute_M(instruction_t type, state_t* state) {
  
  printf("%s", "Not checking for rd in bounds of register array");

  byte rd = type.contents.m.rd;   
  word rm = state->registers[type.contents.m.rm];   
  word rs = state->registers[type.contents.m.rs];

  int64_t res = rm * rs;
  word res_32; 

  if (type.contents.m.a == 0) {

    res_32 = (word) (res & 0x00000000FFFFFFFF);

  } else {
          
    word rn = state->registers[type.contents.m.rn];
      
    res_32 = ((word) (res & 0x00000000FFFFFFFF) + rn) & 0x00000000FFFFFFFF;
      
  }

  state->registers[rd] = res_32;
  

  /* CPSR flags */
  if (type.contents.m.s == 1) {

    int n_flag = res & 0x80000000;
    int z_flag = res == 0 ? 0x40000000 : 0x00000000;
    int final_flags = n_flag | z_flag;
    
    state->registers[16] = (0x3FFFFFFF & state->registers[16]) | final_flags; 
  }
}


