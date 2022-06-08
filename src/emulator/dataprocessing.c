#include <stdbool.h>
#include <stdint.h>

#include "dataprocessing.h"

#define INSDP instruction.contents.dp

void execute_DP(instruction_t instruction, state_t* s) {
  word o1 = s->registers[INSDP.rn];
  word o2;
  if (INSDP.i) {
    o2 = rotate_shift_right(INSDP.operand2 & 0xFF, BIT_MASK(INSDP.operand2, 8, 0xF) * 2);
  } else {
    o2 = shifted_rm(INSDP.operand2, s);
  }
 
  bool write = true;
  word result;
  switch (INSDP.opcode) {
    /* AND */
    case 8:
      write = false;
    case 0:
      result = o1 & o2;
      break;
    /* EOR */
    case 9:
      write = false;
    case 1:
      result = o1 ^ o2;
      break;
    /* SUBTRACT */
    case 10:
      write = false;
    case 2:
      result = o1 - o2;
      break;
    /* REVERSE SUBTRACT */
    case 3:
      result = o2 - o1;
      break; 
    /* ADD */
    case 4:
      result = o1 + o2;
      break;
    /* OR */
    case 12:
      result = o1 | o2;
      break;
    /* MOVE */
    case 13:
      result = o2;
  } 

  if (write) {
    s->registers[INSDP.rd] = result;
  }
  
  if (INSDP.s) {
    word c_flag = 0;
    switch (INSDP.opcode) {
      /* SUBTRACT */
      case 2:
      case 10:
        if (result <= o1) {
          c_flag = 0x20000000;
        }
        break;
      /* REVERSE SUBTRACT */
      case 3:
        if (result <= o2) {
          c_flag = 0x20000000;
        }
        break;
      /* ADD */
      case 4:
        if (result < o1 || result < o2) {
          c_flag = 0x20000000;
        } 
    }
    word z_flag = result ? 0 : 0x40000000;
    word n_flag = result >> (sizeof(word) * 8 - 1) ? 0x80000000 : 0;
    word final_flag = c_flag | z_flag | n_flag;

    s->registers[CPSR] = (s->registers[CPSR] & 0x1FFFFFFF) | final_flag;
  }
}
