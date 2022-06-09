#include "singledatatransfer.h"
#include <string.h>

#define T_SDT t.contents_f.sdt_f
#define T_ADDR t.contents_f.sdt_f.addr
#define T_SHIFT t.contents_f.sdt_f.addr.values_addr_t.shift
#define T_SHIFTED t.contents_f.sdt_f.addr.values_addr_t.shift.values_oper_t.sh_reg
#define SET_BITS(b, n) instruction |= ((b) << (n))

word assemble_SDT(token_t t, byte curr_pc_val, byte n_instructions, stack_t *constants_stack) {
  word instruction = 0;

  SET_BITS(0x01, 26); // Distinguishing bits for SDT instruction

  SET_BITS(T_SDT.rd, 12); // Set Rd

  if (strcmp(T_SDT.expr, "ldr") == 0) {
    SET_BITS(1, 20); // Set L
    if (T_ADDR.i) {
      SET_BITS(1, 25); // Set I
      if (T_ADDR.values_addr_t.immediate <= 0xFF) { 
        // Replace with MOV instruction
        token_t movt;
        movt.format = DP_MOV_F;
        movt.contents_f.dp_mov_f.rd = T_SDT.rd;

        shift_t st;
        st.i = true;
        st.values_oper_t.immediate = T_ADDR.values_addr_t.immediate;

        movt.contents_f.dp_mov_f.operand2 = st;
        return assemble_DP(movt);
      }
      // Add to end of assembled program
      SET_BITS(PC, 16); // Set Rn to PC
      SET_BITS(1, 23); // Set U 

      SET_BITS(1, 24); // Use pre indexing
      SET_BITS(size(constants_stack) + n_instructions - curr_pc_val + 8, 0); // Set offset
      push(constants_stack, T_ADDR.values_addr_t.immediate);
      return instruction;
    }
  }

  SET_BITS(T_ADDR.rn, 16); // Set Rn
  if (T_ADDR.p) {
    SET_BITS(1, 24); // Set P
  }

  if (T_ADDR.i) {
    SET_BITS(1, 25); // Set I
    if (T_ADDR.sighn) {
      SET_BITS(1, 23); // Set U 
    }
    word offset = 0;
    offset |= T_SHIFTED.rm;
    word shift = 0;
    byte shift_type = T_SHIFTED.shift_type; 
    if (shift_type) {
      shift |= 1;
      shift |= T_SHIFTED.shifted_vals_t.rs;
    } else {
      shift |= T_SHIFTED.shifted_vals_t.immediate;
    }
    offset |= shift << 4;
  } else {
    SET_BITS(1, 23); // Set U
    SET_BITS(T_ADDR.values_addr_t.immediate, 0);
  }

  return instruction;
}
