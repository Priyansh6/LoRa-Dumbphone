#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "dataprocessing.h"
#include "utilities.h"

enum InstructionType determine_instruction_type(word w);

instruction_t decode_instruction(word w);

void execute(instruction_t instruction, state_t *s) {
  switch (instruction.type) {
    case DP: execute_DP(instruction, s); break;
    case M: execute_M(instruction, s); break;
    case SDT: execute_SDT(instruction, s); break;
    case B: execute_B(instruction, s); break;
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

word getWord(byte *memory, int n){
  int start = n * sizeof(word);
  return (memory[start] << 24)     |
         (memory[start + 1] << 16) |   
         (memory[start + 2] << 8)  |
         (memory[start + 3]);
}

void load_binary(state_t *s, FILE *fp) {
  fread(&s->memory, sizeof(byte), MEMSIZE, fp);
  
  word *words = (word *) s->memory;

  for (int i = 0; i < MEMSIZE / sizeof(word); i++) {
    swap_endian(&words[i]);
  }
}

int main(int argc, char **argv) {
  
  printf("Trying to open file %s\n", argv[1]);

  FILE *fp;
  fp = fopen(argv[1], "rb");
  if (fp  == NULL) {
    perror("Error opening file!\n");
    exit(EXIT_FAILURE); 
  }

  state_t s;
  init_emulator(&s);
  load_binary(&s, fp);
  
  int i = 0;
  word curr = getWord(s.memory, i);
  while (curr != 0) {
    printf("s.memory[%d]: %x %p\n", i, curr, (void *) &curr);
    i++;
    curr = getWord(s.memory, i);
  }

  return EXIT_SUCCESS;

}
