#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "emulator/dataprocessing.h"
#include "emulator/multiply.h"
#include "emulator/singledatatransfer.h"
#include "emulator/branch.h"
#include "emulator/terminate.h"
#include "utilities.h"

#define DEBUG_STATEMENT(x) if(is_debug) {x;};

bool is_debug = false;

enum InstructionType determine_instruction_type(word w) {
  if (w == 0) {
    return T;
  } 
  if BIT_MASK(w, 26, 1) {
    return SDT;
  }
  if BIT_MASK(w, 27, 1) {
    return B;
  }
  if BIT_MASK(w, 25, 1) {
    return DP;
  }
  if (BIT_MASK(w, 4, 0xF) == 9) { // 9 is the mask for distinguishing between a data processing and multiply instruction
    return M;
  }
  return DP;
}

instruction_t decode_instruction(word w) {
  enum InstructionType it = determine_instruction_type(w);  
  instruction_t instruction;
  instruction.type = it;
  instruction.cond = BIT_MASK(w, 28, 0xF);
   
  if (it == DP) {
    instruction.contents.dp.i = BIT_MASK(w, 25, 1);
    instruction.contents.dp.opcode = BIT_MASK(w, 21, 0xF);
    instruction.contents.dp.s = BIT_MASK(w, 20, 1);
    instruction.contents.dp.rn = BIT_MASK(w, 16, 0xF); 
    instruction.contents.dp.rd = BIT_MASK(w, 12, 0xF);
    instruction.contents.dp.operand2 = w & 0xFFF;
  } else if (it == M) {
    instruction.contents.m.a = BIT_MASK(w, 21, 1);
    instruction.contents.m.s = BIT_MASK(w, 20, 1); 
    instruction.contents.m.rd = BIT_MASK(w, 16, 0xF);
    instruction.contents.m.rn = BIT_MASK(w, 12, 0xF);
    instruction.contents.m.rs = BIT_MASK(w, 8, 0xF);
    instruction.contents.m.rm = w & 0xF;
  } else if (it == SDT) {
    instruction.contents.sdt.i = BIT_MASK(w, 25, 1); 
    instruction.contents.sdt.p = BIT_MASK(w, 24, 1);
    instruction.contents.sdt.u = BIT_MASK(w, 23, 1);
    instruction.contents.sdt.l = BIT_MASK(w, 20, 1);
    instruction.contents.sdt.rn = BIT_MASK(w, 16, 0xF);
    instruction.contents.sdt.rd = BIT_MASK(w, 12, 0xF);
    instruction.contents.sdt.offset = w & 0xFFF;
  } else if (it == B) {
    instruction.contents.b.offset = w & 0xFFFFFF;
  }

  return instruction;
}

bool cond_true(byte cond, word cpsr) {
  byte n = BIT_MASK(cpsr, 31, 1);
  byte z = BIT_MASK(cpsr, 30, 1);
  byte v = BIT_MASK(cpsr, 28, 1);
  bool res;
  switch (cond) {
    case 0: res = z; break;
    case 1: res = !z; break;
    case 10: res =  n == v; break;
    case 11: res = n != v; break;
    case 12: res = !z && (n == v); break;
    case 13: res = z || (n != v); break;
    default: res = true;
  }
  return res;
}

int execute(instruction_t instruction, state_t *s) {
  DEBUG_STATEMENT(pprint_instruction_t(instruction)); 

  if (instruction.type == T) {
    execute_T(instruction, s);
  }

  if (!cond_true(instruction.cond, s->registers[CPSR])) {
    return 1;
  }

  switch (instruction.type) {
    case DP: execute_DP(instruction, s); break;
    case M: execute_M(instruction, s); break;
    case SDT: execute_SDT(instruction, s); break;
    case B: execute_B(instruction, s); break;
    case T: execute_T(instruction, s); break;
  }
  
  return 0;
}

state_t *init_state() {
  state_t *s = (state_t *) calloc(1, sizeof(state_t));
  if (s == NULL) {
    perror("Couldn't allocate memory for state!");
    exit(EXIT_FAILURE);
  }
  return s;
  //memset(s->memory, 0, MEMSIZE * sizeof(byte));
  //memset(s->registers, 0, NOREGS * sizeof(word));
}

void load_binary(state_t *s, FILE *fp) {
  fread(&s->memory, sizeof(byte), MEMSIZE, fp);
}

int main(int argc, char **argv) {
  
  DEBUG_STATEMENT(printf("Trying to open file %s\n", argv[1]));

  FILE *fp;
  fp = fopen(argv[1], "rb");
  if (fp  == NULL) {
    perror("Error opening file!\n");
    exit(EXIT_FAILURE); 
  }

  state_t *s = init_state();
  load_binary(s, fp);

  fclose(fp);
  

  DEBUG_STATEMENT(
    int i = 0;
    byte curr = s->memory[i]; 
    while (get_word(s->memory, i)) {
      printf("s.memory[%d]: %x %p\n", i, curr, (void *) &curr);
      i++; 
      curr = s->memory[i];
    }
  );

  word fetch;
  word fetch_tmp;

  instruction_t decode; 
  instruction_t decode_tmp;

  int status = 0;

  bool has_fetched = false;
  bool has_decoded = false;
  
  int iter = 0;
  while (true) {
    DEBUG_STATEMENT(
      printf("Iter: %d\n", iter);
      pprint_state_t(*s);
      printf("PC: %d\n", s->registers[PC]);
    );

    fetch_tmp = fetch;
    fetch = get_word(s->memory, s->registers[PC]);

    DEBUG_STATEMENT(printf("Fetched: %x\n", fetch));

    decode_tmp = decode; 
    if (has_fetched) {
      decode = decode_instruction(fetch_tmp);
      if (has_decoded) {
        status = execute(decode_tmp, s);
      }
      has_decoded = true;
    }
    has_fetched = true;
    if (decode_tmp.type == B && status == 0) {
      has_decoded = false;
      has_fetched = false;
    } else {
      s->registers[PC] += 4;
      iter++;
    }
    DEBUG_STATEMENT(printf("\n\n"));
  }
  return EXIT_SUCCESS;
}
