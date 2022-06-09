#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataprocessing.h"

#define SET_OPCODE(op) result |= (op << 21)
#define SET_RN(r) result |= (r << 16)
#define SET_RD(r) result |= (r << 12)
#define COMP token.contents_f.dp_comp_f
#define MOV token.contents_f.dp_mov_f
#define NCOMP token.contents_f.dp_ncomp_f

word assemble_DP(token_t token) {
  word result = 14 << 28; // Setting condition code to 14
  
  char *o2;
  switch (token.format) {
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
        SET_OPCODE(10);
      }
      
      SET_RD(strtoul(COMP.rd + 1, NULL, 10));
      SET_RN(strtoul(COMP.rn + 1, NULL, 10));

      o2 = COMP.operand2;
      break;
    case DP_MOV_F:
      SET_OPCODE(11);

      SET_RD(strtoul(MOV.rd + 1, NULL, 10));

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

      SET_RN(strtoul(NCOMP.rn + 1, NULL, 10));
      
      result |= (1 << 20); // Setting S bit

      o2 = NCOMP.operand2;
      break;
    default:
      printf("Illegal instruction type to assemble");
      return 0;
  }  

  if (o2[0] != 'r') {
    word shift_rm = 0;

    char *rest;
    shift_rm |= strtoul(o2 + 1, &rest, 10);
    
    if (strlen(rest) > 3) {
      rest += 2;
      
      byte shift_type;
      if (!strncmp(rest, "lsl", 3)) {
        shift_type = 0;
      }
      else if (!strncmp(rest, "lsr", 3)) {
        shift_type = 1;
      }
      else if (!strncmp(rest, "asr", 3)) {
        shift_type = 2;
      }
      else if (!strncmp(rest, "ror", 3)) {
        shift_type = 3;
      }
      shift_rm |= (shift_type << 5);

      rest += 4;
      if (rest[0] == 'r') {
        shift_rm |= (1 << 4);
        shift_rm |= (strtoul(rest + 1, NULL, 10) << 8);
      } else {
        shift_rm |= (toimmediate(rest) << 7);
      }
    
      result |= shift_rm;
    }
  } else {
    result |= (1 << 25); // Setting I bit
    result |= toimmediate(o2);
  }

  return result;
}
