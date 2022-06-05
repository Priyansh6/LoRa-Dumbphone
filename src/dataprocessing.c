#include <stdbool.h>
#include <stdint.h>

#include "utilities.h"

void execute_DP(instruction_t instruction, state_t* s) {
  word o1 = s->registers[instruction.contents.dp.rn];
  word o2;
  if (instruction.contents.dp.i) {
    o2 = rotate_shift_right((word) instruction.contents.dp.operand2 & 0xFF, 
        (word) (instruction.contents.dp.operand2 >> 8) * 2);
  } else {
    o2 = shifted_rm(instruction.contents.dp.operand2, s);
  }

  word result;
  switch(instruction.contents.dp.opcode) {
    /* AND */
    case 0:
    case 8:
      result = o1 & o2;
      break;
    /* EOR */
    case 1:
    case 9:
      result = o1 ^ o2;
      break;
    /* SUBTRACT */
    case 2:
    case 10:
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
      break;
  } 

  switch(instruction.contents.dp.opcode) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 12:
    case 13:
    s->registers[instruction.contents.dp.rd] = result;
  }
  
  if (instruction.contents.dp.s) {
    word c_flag = 0;
    switch(instruction.contents.dp.opcode) {
      case 2:
      case 10:
        if (result <= o1) {
          c_flag = 0x20000000;
        }
        break;
      case 3:
        if (result <= o2) {
          c_flag = 0x20000000;
        }
        break;
      case 4:
        if (result < o1 || result < o2) {
          c_flag = 0x20000000;
        } 
    }
    word z_flag = result ? 0 : 0x40000000;
    word n_flag = result >> (sizeof(word) * 8 - 1) ? 0x80000000 : 0;
    word final_flag = c_flag | z_flag | n_flag;

    s->registers[16] = (s->registers[16] & 0x1FFFFFFF) | final_flag;
  }
}
