#include <stdint.h>
#include <stdio.h>

#include "multiply.h"

#define INSM instruction.contents.m

void execute_M(instruction_t instruction, state_t *s) {

  byte rd = INSM.rd;   
  word rm = s->registers[INSM.rm];   
  word rs = s->registers[INSM.rs];

  int64_t res = rm * rs;
  word res_32 = (word) res; 

  if (instruction.contents.m.a) {
    res_32 += s->registers[INSM.rn];
  }
  
  s->registers[rd] = res_32;
  
  /* CPSR flags */
  if (INSM.s == 1) {
    int n_flag = res & 0x80000000;
    int z_flag = res == 0 ? 0x40000000 : 0;
    int final_flags = n_flag | z_flag;

    s->registers[CPSR] = (0x3FFFFFFF & s->registers[CPSR]) | final_flags; 
  }
}
