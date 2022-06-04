#include <stdio.h>
#include <stdint.h>

#include "utilities.h"

void execute_M(instruction_t type, state_t* state) {
  
  printf("%s", "Not checking for rd in bounds of register array");

  byte rd = type.contents_u.ms.rd;   
  word rm = state.registers[type.contents_u.ms.rm];   
  word rs = state.registers[type.contents_u.ms.rs];

  int64_t res = rm * rs;

  if (type.contents_u.ms.a == 0) {

    32res = (word) (res & 0x00000000FFFFFFFF);

  } else {
          
    word rn = state.registers[type.contents_u.ms.rn];
      
    32res = ((word) (res & 0x00000000FFFFFFFF) + rn) & 0x00000000FFFFFFFF;
      
  }

  state.registersp[rd] = 32res;
  

  /* CPSR flags */
  if (type.contents_u.ms.s == 1) {

    int n_flag = res & 0x80000000;
    int z_flag = res == 0 ? 0x4FFFFFFF : 0x0FFFFFFF;
    int final_flags = n_flag | z_flag;

    state.registers[16] = state.registers[16] & final_flags; 
  }
}


