#include <stdio.h>

#include "utilities.h"

void update_cpsr(word modified_register, int c_bit);

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
      printf("PC  :% 11d (0x%010x)\n", state.registers[i], state.registers[i]);
    } else if (i == CPSR) {
      printf("CPSR:% 11d (0x%010x)\n", state.registers[i], state.registers[i]);
    } else {
      printf("$%-3d:% 11d (0x%010x)\n", i, state.registers[i], state.registers[i]);
    }
  }
  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMSIZE; i += 4) {
    if (state.memory[i] != 0) {
      printf("%#010x: %#010x\n", i, get_word(state.memory, i));//state.memory[i]);
    }
  }
}

word get_word(byte *memory, int n){
  return (memory[n + 3] << 24) |
         (memory[n + 2] << 16) |
         (memory[n + 1] << 8)  |
         (memory[n]);
}

