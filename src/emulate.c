#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dataprocessing.h"
#include "multiply.h"
#include "singledatatransfer.h"
#include "branch.h"
#include "terminate.h"
#include "utilities.h"

#define DEBUG_STATEMENT(x) if(is_debug) {x;};

bool is_debug = false;

enum InstructionType determine_instruction_type(word w) {
  if (w == 0) {
    return T;
  } 
  if (w >> 26 & 1) {
    return SDT;
  }
  if (w >> 27 & 1) {
    return B;
  }
  if (w >> 25 & 1) {
    return DP;
  }
  if ((w >> 4 & 0xF) == 9) {
    return M;
  }
  return DP;
}

instruction_t decode_instruction(word w) {
  enum InstructionType it = determine_instruction_type(w);  
  instruction_t instruction;
  instruction.type = it;
  instruction.cond = (w >> 28) & 0xF;
   
  if (it == DP) {
    instruction.contents.dp.i = (w >> 25) & 1;
    instruction.contents.dp.opcode = (w >> 21) & 0xF;
    instruction.contents.dp.s = (w >> 20) & 1;
    instruction.contents.dp.rn = (w >> 16) & 0xF; 
    instruction.contents.dp.rd = (w >> 12) & 0xF;
    instruction.contents.dp.operand2 = w & 0xFFF;  
  } else if (it == M) {
    instruction.contents.m.a = (w >> 21) & 1;
    instruction.contents.m.s = (w >> 20) & 1; 
    instruction.contents.m.rd = (w >> 16) & 0xF;
    instruction.contents.m.rn = (w >> 12) & 0xF;
    instruction.contents.m.rs = (w >> 8) & 0xF;
    instruction.contents.m.rm = w & 0xF;
  } else if (it == SDT) {
    instruction.contents.sdt.i = (w >> 25) & 1; 
    instruction.contents.sdt.p = (w >> 24) & 1;
    instruction.contents.sdt.u = (w >> 23) & 1;
    instruction.contents.sdt.l = (w >> 20) & 1;
    instruction.contents.sdt.rn = (w >> 16) & 0xF;
    instruction.contents.sdt.rd = (w >> 12) & 0xF;
    instruction.contents.sdt.offset = w & 0xFFF;
  } else if (it == B) {
    instruction.contents.b.offset = w & 0xFFFFFF;
  }

  return instruction;
}

bool cond_true(byte cond, word cpsr) {
  byte n = (cpsr >> 31) & 1;
  byte z = (cpsr >> 30) & 1;
  byte v = (cpsr >> 28) & 1;
  bool res;
  switch (cond) {
    case 0: res = n; break;
    case 1: res = !n; break;
    case 10: res =  n == v; break;
    case 11: res = n != v; break;
    case 12: res = !z && (n == v); break;
    case 13: res = z || (n != v); break;
    case 14: 
    default: res = true;
  }
  return res;
}

void execute(instruction_t instruction, state_t *s) {
  DEBUG_STATEMENT(pprint_instruction_t(instruction)); 
  if (instruction.type == T) {
    execute_T(instruction, s);
  }

  if (!cond_true(instruction.cond, s->registers[CPSR])) {
    return;
  }
  switch (instruction.type) {
    case DP: execute_DP(instruction, s); break;
    case M: execute_M(instruction, s); break;
    case SDT: execute_SDT(instruction, s); break;
    case B: execute_B(instruction, s); break;
    case T: execute_T(instruction, s); break;
    default:
      pprint_instruction_t(instruction); 
  }
}

void init_emulator(state_t *s) {
  memset(s->memory, 0, MEMSIZE * sizeof(byte));
  memset(s->registers, 0, NOREGS * sizeof(word));
}

void swap_endian(word *x) {
  *x = (*x>>24) |
       ((*x<<8) & 0x00FF0000) |
       ((*x>>8) & 0x0000FF00) |
       (*x<<24);
}

void load_binary(state_t *s, FILE *fp) {
  fread(&s->memory, sizeof(byte), MEMSIZE, fp);
  
  word *words = (word *) s->memory;

  for (int i = 0; i < MEMSIZE / sizeof(word); i++) {
    swap_endian(&words[i]);
  }
}

int main(int argc, char **argv) {
  
  DEBUG_STATEMENT(printf("Trying to open file %s\n", argv[1]));

  FILE *fp;
  fp = fopen(argv[1], "rb");
  if (fp  == NULL) {
    perror("Error opening file!\n");
    exit(EXIT_FAILURE); 
  }

  state_t s;
  init_emulator(&s);
  load_binary(&s, fp);
  

  DEBUG_STATEMENT(int i = 0;
  byte curr = s.memory[i]; 
  while (curr != 0) {
    printf("s.memory[%d]: %x %p\n", i, curr, (void *) &curr);
    curr = s.memory[i];
    i++;
  });

  word fetch;
  word fetch_tmp;
  instruction_t decode; 
  instruction_t decode_tmp;

  bool has_fetched = false;
  bool has_decoded = false;
  
  int iter = 0;
  while (true) {
    DEBUG_STATEMENT(printf("Iter: %d\n", iter));
    DEBUG_STATEMENT(pprint_state_t(s));
    DEBUG_STATEMENT(printf("PC: %d\n", s.registers[PC]));
    fetch_tmp = fetch;
    fetch = get_word_raw(s.memory, s.registers[PC]);
    DEBUG_STATEMENT(printf("Fetched: %x\n", fetch));
    decode_tmp = decode; 
    if (has_fetched) {
      decode = decode_instruction(fetch_tmp);
      if (has_decoded) {
        execute(decode_tmp, &s);
      }
      has_decoded = true;
    }
    has_fetched = true;
    s.registers[PC] += 4;
    iter++;
    DEBUG_STATEMENT(printf("\n\n"));
  }
  
  return EXIT_SUCCESS;

}
