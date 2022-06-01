#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MEMSIZE 64 * 1024
#define NOREGS 17

typedef uint8_t byte;
typedef uint32_t word;

typedef struct state {
  byte memory[MEMSIZE];
  word registers[NOREGS];
} state_t;

enum InstructionType { DP, M, SDT, B }

typedef struct instruction {
  enum InstructionType type;
  byte cond;
  union {
    struct { byte i, opcode, s, rn, rd; uint16_t operand2;  };  
    struct { byte a, s, rd, rn, rs, rm; };  
    struct { byte i, p, u, l, rn, rd; uint16_t offset; };  
    struct { word offset; };  
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
