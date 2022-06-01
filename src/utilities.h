#ifndef UTILITIES_H
#define UTILITIES_H

#define MEMSIZE 64 * 1024
#define NOREGS 17

typedef uint8_t byte;
typedef uint32_t word;

typedef struct state {
  byte memory[MEMSIZE];
  word registers[NOREGS];
} state_t;

enum InstructionType { DP, M, SDT, B };

typedef struct instruction {
  enum InstructionType type;
  byte cond;
  union {
    struct { byte i, opcode, s, rn, rd; uint16_t operand2;  };  
    struct { byte a, s, rd, rn, rs, rm; };  
    struct { byte i, p, u, l, rn, rd; uint16_t offset; };  
    struct { word offset; };  
  }  
} instruction_t;

#endif
