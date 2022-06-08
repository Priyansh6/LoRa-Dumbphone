#include <ctype.h>
#include <stdio.h>

#include "utilities.h"

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
