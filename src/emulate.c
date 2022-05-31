#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MEMSIZE 64 * 1024

typedef uint8_t byte;
typedef uint32_t word;

typedef struct state {
  byte memory[MEMSIZE];
  word registers[17];
} state_t;

void init_emulator(state_t *s) {
  memset(s->memory, 0, MEMSIZE * sizeof(byte));
  memset(s->registers, 0, 17 * sizeof(word));
}

void swap_endian(word *x) {
  *x = (*x>>24) |
       ((*x<<8) & 0x00FF0000) |
       ((*x>>8) & 0x0000FF00) |
       (*x<<24);
}

void load_binary(state_t *s, FILE *fp) {
  fread(&s->memory, 1, MEMSIZE, fp);
  
  word *words = (word *) s->memory;

  for (int i = 0; i < MEMSIZE / 4; i++) {
    swap_endian(&words[i]);
  }
}

int main(int argc, char **argv) {
  
  printf("Trying to open file %s\n", argv[1]);

  FILE *fp;
  if ((fp = fopen(argv[1], "rb")) == NULL) {
    perror("Error opening file!\n");
    exit(EXIT_FAILURE); 
  }

  state_t s;
  init_emulator(&s);
  load_binary(&s, fp);
  
  int i = 0;
  while (s.memory[i] != 0) {
    printf("s.memory[%d]: %x %p\n", i, s.memory[i], (void *) &s.memory[i]);
    i++;
  }

  return EXIT_SUCCESS;
}
