#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataprocessing.h"

#define SET_OPCODE(op) result |= (op << 21)
#define SET_RN(r) result |= (r << 16)
#define SET_RD(r) result |= (r << 12)
#define COMP t.contents_f.dp_comp_f
#define MOV t.contents_f.dp_mov_f
#define NCOMP t.contents_f.dp_ncomp_f
#define O2_SH_REG o2.values_oper_t.sh_reg

word assemble_DP(token_t t) {
  word result = 14 << 28; // Setting condition code to 14
  
  shift_t o2;
  switch (t.format) {
    case DP_COMP_F:
      if (!strcmp(COMP.opcode, "and")) {
        SET_OPCODE(0);
      }
      else if (!strcmp(COMP.opcode, "eor")) {
        SET_OPCODE(1);
      }
      else if (!strcmp(COMP.opcode, "sub")) {
        SET_OPCODE(2);
      }
      else if (!strcmp(COMP.opcode, "rsb")) {
        SET_OPCODE(3);
      }
      else if (!strcmp(COMP.opcode, "add")) {
        SET_OPCODE(4);
      }
      else if (!strcmp(COMP.opcode, "orr")) {
        SET_OPCODE(12);
      }
      
      SET_RD(COMP.rd);
      SET_RN(COMP.rn);

      o2 = COMP.operand2;
      break;
    case DP_MOV_F:
      SET_OPCODE(13);

      SET_RD(MOV.rd);

      o2 = MOV.operand2;
      break;
    case DP_NCOMP_F:
      if (!strcmp(NCOMP.opcode, "tst")) {
        SET_OPCODE(8);   
      }
      else if (!strcmp(NCOMP.opcode, "teq")) {
        SET_OPCODE(9); 
      }
      else if (!strcmp(NCOMP.opcode, "cmp")) {
        SET_OPCODE(10);
      }

      SET_RN(NCOMP.rn);
      
      result |= (1 << 20); // Setting S bit

      o2 = NCOMP.operand2;
      break;
    default:
      printf("Illegal instruction type to assemble");
      exit(EXIT_FAILURE);
  }  
  printf("%x\n", o2.i);
  if (o2.i == 1) {
    result |= (1 << 25); // Setting I bit
    result |= o2.values_oper_t.immediate;
  } else {
    result |= O2_SH_REG.rm;
    result |= (O2_SH_REG.shift_type << 5);
    if (O2_SH_REG.is_reg) {
      result |= (1 << 4);
      result |= (O2_SH_REG.shifted_vals_t.rs << 8);
    } else {
      result |= (O2_SH_REG.shifted_vals_t.immediate << 7);
    }
  }
  
  return result;
}


