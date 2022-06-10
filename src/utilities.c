#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utilities.h"

/*
bool isnumber(char *str, byte base) {
  char max_digit = base <= 10 ? '0' + base - 1 : 'A' + base - 11;
  for (char *x = str; *x; x++) {
    char upper = isalpha(*x) ? toupper(*x) : *x;
    if (upper < '0' || upper > max_digit || (upper > '9' && upper < 'A')) {
      return false;
    }
  }
  return true;
}

word toimmediate(char *str) {
  str++;
  return strlen(str) >= 2 && str[1] == 'x' ? strtoul(str + 2, NULL, 16) : strtoul(str, NULL, 10);
}*/

word shifted_rm(uint16_t shift_rm, state_t* s) {
  word rm = s->registers[shift_rm & 0xF];

  byte shift = BIT_MASK(shift_rm, 4, 0xFF);
  byte shift_type = BIT_MASK(shift, 1, 3);

  word shift_amount;
  if (shift & 1) {
    shift_amount = s->registers[shift >> 4];
  } else {
    shift_amount = shift >> 3;
  }

  switch (shift_type) {
    case 0:
      return rm << shift_amount;
    case 1:
      return rm >> shift_amount;
    case 2:
      return (int32_t) rm >> shift_amount;
    default:
      return rotate_shift_right(rm, shift_amount);
  }
}

word rotate_shift_right(word x, word shift_amount) {
  shift_amount %= sizeof(word) * 8;
  return (x >> shift_amount) | (x << (sizeof(word) * 8 - shift_amount));
}

void pprint_instruction_t(instruction_t instruction) {
  printf("Instruction\n");
  printf("Type: ");
  switch (instruction.type) {
    case DP: printf("DP\n"); break;
    case M: printf("M\n"); break;
    case SDT: printf("SDT\n"); break; 
    case B: printf("B\n"); break;
    case T: printf("T\n"); break;
  }
  printf("Cond: %x\n", instruction.cond);
  if (instruction.type == DP) {
    printf("I: %x\n", instruction.contents.dp.i);
    printf("Opcode: %x\n", instruction.contents.dp.opcode);
    printf("S: %x\n", instruction.contents.dp.s);
    printf("RN: %x\n", instruction.contents.dp.rn);
    printf("RD: %x\n", instruction.contents.dp.rd);
    printf("Operand2: %x\n", instruction.contents.dp.operand2);
  } else if (instruction.type == M) {
    printf("A: %x\n", instruction.contents.m.a);
    printf("S: %x\n", instruction.contents.m.s);
    printf("RD: %x\n", instruction.contents.m.rd);
    printf("RN: %x\n", instruction.contents.m.rn);
    printf("RS: %x\n", instruction.contents.m.rs);
    printf("RM: %x\n", instruction.contents.m.rm);
  } else if (instruction.type == SDT) {
    printf("I: %x\n", instruction.contents.sdt.i);
    printf("P: %x\n", instruction.contents.sdt.p);
    printf("U: %x\n", instruction.contents.sdt.u);
    printf("L: %x\n", instruction.contents.sdt.l);
    printf("RN: %x\n", instruction.contents.sdt.rn);
    printf("RD: %x\n", instruction.contents.sdt.rd);
    printf("Offset: %x\n", instruction.contents.sdt.offset);
  } else if (instruction.type == B) {
    printf("Offset: %x\n", instruction.contents.b.offset);
  }
  printf("\n\n");
}

void pprint_state_t(state_t state) {
  printf("Registers:\n");
  for (int i = 0; i < NOREGS; i++) {
    if (i == PC) {
      printf("PC  :");
    } else if (i == CPSR) {
      printf("CPSR:");
    } else if (i == 14 || i == 13){
      continue;
    } else {
      printf("$%-3d:", i);
    } 
    if ((int) state.registers[i] < 0){
      printf(" % 10d (0x%08x)\n", state.registers[i], state.registers[i]);
    } else {
      printf("%11d (0x%08x)\n", state.registers[i], state.registers[i]);
    }
  }
  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMSIZE; i += 4) {
    if (get_word_raw(state.memory, i) != 0) {
      printf("0x%08x: 0x%08x\n", i, get_word_raw(state.memory, i));
    }
  }
}

word get_word(byte *memory, int n){
  return (memory[n + 3] << 24) |
         (memory[n + 2] << 16) |
         (memory[n + 1] << 8)  |
         (memory[n]);
}

word get_word_raw(byte *memory, int n){
  return (memory[n] << 24) |
         (memory[n + 1] << 16) |
         (memory[n + 2] << 8)  |
         (memory[n + 3]);
}

void set_word(byte *memory, int addr, word w) {
  memory[addr] = (byte) w;
  memory[addr + 1] = (byte) (w >> 8);
  memory[addr + 2] = (byte) (w >> 16);
  memory[addr + 3] = (byte) (w >> 24);
}


void pprint_shifted_reg(shifted_reg_t srt) {
    printf("          SHIFTED REG\n");
    printf("            is_reg : %s\n", BOOL_STR(srt.is_reg));
    printf("            shift_type : %x\n", srt.shift_type);
    printf("            RM : %x\n", srt.rm);
    if (srt.is_reg){
      printf("            RS : %x\n", srt.shifted_vals_t.rs);
    } else {
      printf("            IMMED : %x\n", srt.shifted_vals_t.immediate);
    }
}

void pprint_shift(shift_t srt) {
    printf("         SHIFT\n");
    printf("           i : %s\n", BOOL_STR(srt.i));
    if (!srt.i){
      pprint_shifted_reg(srt.values_oper_t.sh_reg);
    } else {
      printf("           IMMED : %x\n", srt.values_oper_t.immediate);
    }
}


void pprint_addr(address_s_t srt){
    printf("        ADDR\n");
    printf("          p : %s\n", BOOL_STR(srt.p));
    printf("          i : %s\n", BOOL_STR(srt.i));
    printf("          sighn : %s\n", BOOL_STR(srt.sighn));
    if (srt.i){
      printf("          IMMED : %x\n", srt.values_addr_t.immediate);
    } else {
      printf("          RN : %x\n", srt.rn);
      pprint_shift(srt.values_addr_t.shift);
    }
}

void pprint_M(token_t token) {
  printf("Intruction Type DP\n");
  if (token.format == M_F) {
    printf("  RD : %x\n", token.contents_f.m_f.rd);
    printf("  RM : %x\n", token.contents_f.m_f.rm);
    printf("  RS : %x\n", token.contents_f.m_f.rs);
  }  else {
    printf("  RD : %x\n", token.contents_f.ma_f.rd);
    printf("  RM : %x\n", token.contents_f.ma_f.rm);
    printf("  RS : %x\n", token.contents_f.ma_f.rs);
    printf("  RN : %x\n", token.contents_f.ma_f.rn);
  }
}

void pprint_DP(token_t token){
  printf("Intruction Type DP\n");
  switch (token.format) {
    case DP_COMP_F:
      printf("  OPCODE : %s\n", token.contents_f.dp_comp_f.opcode);
      printf("  RD : %x\n", token.contents_f.dp_comp_f.rd);
      printf("  Rn : %x\n", token.contents_f.dp_comp_f.rn);
      pprint_shift(token.contents_f.dp_comp_f.operand2);
      break; 
    case DP_MOV_F:
      printf("  RD : %x\n", token.contents_f.dp_mov_f.rd);
      pprint_shift(token.contents_f.dp_mov_f.operand2);
      break; 
    case DP_NCOMP_F:
      printf("  OPCODE : %s\n", token.contents_f.dp_ncomp_f.opcode);
      printf("  Rn : %x\n", token.contents_f.dp_ncomp_f.rn);
      pprint_shift(token.contents_f.dp_ncomp_f.operand2);
      break; 
    default:
    printf("INV LOCATION\n");
  }
}

void pprint_SDT(token_t token){
  printf("Intruction Type DP\n");
  printf("  EXPR : %s\n", token.contents_f.sdt_f.expr);
  printf("  RD : %x\n", token.contents_f.sdt_f.rd);
  pprint_addr(token.contents_f.sdt_f.addr);
}

void pprint_B(token_t t){
  printf("Intruction Type BRANCH\n");
  printf("  COND : %s\n", t.contents_f.b_f.cond);
  printf("  OFFSET : %s\n", t.contents_f.b_f.offset);
}



void pprint_token(token_t t){

  switch (t.format) {
      case DP_COMP_F: 
      case DP_MOV_F: 
      case DP_NCOMP_F:
        pprint_DP(t);
        break;
      
      case M_F: 
      case MA_F:
        pprint_M(t);
        break;
      
      case SDT_F:
        pprint_SDT(t);
        break;

      case B_F:
        pprint_B(t); 
        break;
        
      case ANDEQ_F:
        printf("TERMINATE\n");
        break;

      case LSL_F:
        printf("LSL_F\n");
        printf(" RN : %x\n", t.contents_f.lsl_f.rn);
        printf(" EXPR : %x\n", t.contents_f.lsl_f.expr);
        break;

      case INV_F: 
      default:
        printf("NOT DEFINED PRINT\n");
        break;
    }  

}