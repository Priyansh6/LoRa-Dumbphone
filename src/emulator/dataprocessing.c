#include <stdbool.h>
#include <stdint.h>

#include "dataprocessing.h"

void execute_DP(instruction_t instruction, state_t* s) {
  word o1 = s->registers[instruction.contents.dp.rn];
  word o2;
  if (instruction.contents.dp.i) {
    o2 = rotate_shift_right((word) instruction.contents.dp.operand2 & 0xFF, 
        (word) (instruction.contents.dp.operand2 >> 8) * 2);
  } else {
    o2 = shifted_rm(instruction.contents.dp.operand2, s);
  }
 
  bool write = true;
  word result;
  switch (instruction.contents.dp.opcode) {
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
    s->registers[instruction.contents.dp.rd] = result;
  }
  
  if (instruction.contents.dp.s) {
    word c_flag = 0;
    switch (instruction.contents.dp.opcode) {
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
