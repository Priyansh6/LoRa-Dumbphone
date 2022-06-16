#include "singledatatransfer.h"
#include <string.h>
#include <stdio.h>

#define T_SDT t.contents_f.sdt_f
#define T_ADDR t.contents_f.sdt_f.addr
#define T_SHIFT t.contents_f.sdt_f.addr.values_addr_t.shift
#define T_SHIFTED t.contents_f.sdt_f.addr.values_addr_t.shift.values_oper_t.sh_reg
#define SET_BITS(b, n) instruction |= ((b) << (n))

word assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack) {
  word instruction = 14 << 28;

  SET_BITS(1, 26); // Distinguishing bits for SDT instruction

  SET_BITS(T_SDT.rd, 12); // Set Rd

  if (strcmp(T_SDT.expr, "ldr") == 0) {
    SET_BITS(1, 20); // Set L 

    if (T_ADDR.i) {
      if (T_ADDR.values_addr_t.immediate <= 0xFF) { 
        token_t movt;
        movt.format = DP_MOV_F;
        movt.contents_f.dp_mov_f.rd = T_SDT.rd;

        shift_t st;
        st.i = true;
        st.values_oper_t.immediate = T_ADDR.values_addr_t.immediate;

        movt.contents_f.dp_mov_f.operand2 = st;
        return assemble_DP(movt);
      }
      SET_BITS(PC, 16); // Set Rn to PC
      SET_BITS(1, 23); // Set U 
      SET_BITS(1, 24); // Use pre indexing
      address addr = (size(constants_stack) * 4) + (n_instructions * 4) - curr_pc_val - 8;
      SET_BITS(addr, 0); // Set offset
      push(constants_stack, T_ADDR.values_addr_t.immediate);
      return instruction ;
    }
  }

  SET_BITS(T_ADDR.rn, 16); // Set Rn

  if (T_ADDR.p) {
    SET_BITS(1, 24);
  }

  if (T_ADDR.i) {
    if (T_ADDR.sighn) {
      SET_BITS(1, 23); // Set U 
    }
  } else {
      if(!T_SHIFT.i){
        SET_BITS(1, 25);
        word offset = 0;
        offset |= T_SHIFTED.rm;
        word shift = 0;
        byte shift_type = T_SHIFTED.shift_type; 
        if (T_SHIFTED.is_reg) {
          shift |= 1;
          shift |= T_SHIFTED.shifted_vals_t.rs << 4;
        } else {
          shift |= T_SHIFTED.shifted_vals_t.immediate << 3;
        }
        shift |= shift_type << 1; 
        offset |= shift << 4;
        SET_BITS(offset, 0);
      } 

    if (T_ADDR.sighn){
      SET_BITS(1, 23);
    } // Set U

    SET_BITS(T_SHIFT.values_oper_t.immediate & 0xFFF, 0);
  }

  return instruction;
}
