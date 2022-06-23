#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataprocessing.h"

#define SET_OPCODE(op) instruction |= (op << 21)
#define SET_RN(r) instruction |= (r << 16)
#define SET_RD(r) instruction |= (r << 12)
#define COMP t.contents_f.dp_comp_f
#define MOV t.contents_f.dp_mov_f
#define NCOMP t.contents_f.dp_ncomp_f
#define O2_SH_REG o2.values_oper_t.sh_reg

word assemble_DP(token_t t) {
  word instruction = 14 << 28; // Setting condition code to 14
  
  shift_t o2;
  switch (t.format) {
    case DP_COMP_F:
      if (!strcmp(COMP.opcode, "and")) {
        SET_OPCODE(0);
      } else if (!strcmp(COMP.opcode, "eor")) {
        SET_OPCODE(1);
      } else if (!strcmp(COMP.opcode, "sub")) {
        SET_OPCODE(2);
      } else if (!strcmp(COMP.opcode, "rsb")) {
        SET_OPCODE(3);
      } else if (!strcmp(COMP.opcode, "add")) {
        SET_OPCODE(4);
      } else if (!strcmp(COMP.opcode, "orr")) {
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
      } else if (!strcmp(NCOMP.opcode, "teq")) {
        SET_OPCODE(9); 
      } else if (!strcmp(NCOMP.opcode, "cmp")) {
        SET_OPCODE(10);
      }

      SET_RN(NCOMP.rn);
      SET_BITS(instruction, 1, 20); // Setting S bit
      o2 = NCOMP.operand2;
      break;
    
    default:
      printf("Illegal instruction type to assemble\n");
      exit(EXIT_FAILURE);
  }  
  
  if (o2.i == 1) {
    SET_BITS(instruction, 1, 25); // Setting I bit

    if (0xFFFFFF00 & o2.values_oper_t.immediate) { // Immediate may need to be rotated
      bool last_valid = false;
      word shift = 0;
      word new_imm = 0;
      
      for (int shift_amount = 2; shift_amount < 32; shift_amount += 2) {
        word rot_imm = rotate_shift_right(o2.values_oper_t.immediate, shift_amount); 
        bool valid = !(rot_imm & 0xFFFFFF00);
        
        if (!valid && last_valid) {
          break;
        }

        if (valid) {
          shift = 32 - shift_amount;
          new_imm = rot_imm;
          last_valid = true;
        }
      }
      
      if (last_valid) {
        SET_BITS(instruction, new_imm, 0);
        SET_BITS(instruction, shift, 7);
      } else {
        printf("Impossible to assemble instruction with specified immediate value\n");
        exit(EXIT_FAILURE);
      }
    } else { // Immediate is already in correct form
      SET_BITS(instruction, o2.values_oper_t.immediate, 0);
    }

  } else {
    SET_BITS(instruction, O2_SH_REG.rm, 0);
    SET_BITS(instruction, O2_SH_REG.shift_type, 5);
    if (O2_SH_REG.is_reg) {
      SET_BITS(instruction, 1, 4);
      SET_BITS(instruction, O2_SH_REG.shifted_vals_t.rs, 8);
    } else {
      SET_BITS(instruction, O2_SH_REG.shifted_vals_t.immediate, 7);
    }
  }

  return instruction;
}


