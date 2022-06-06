#ifndef UTILITIES_H
#define UTILITIES_H

#define MEMSIZE 64 * 1024
#define NOREGS 17
#define PC 15
#define CPSR 16
#define GPIOSIZE 12
#define GPIO_CLEAR 0x20200028
#define GPIO_SET 0x2020001C
#define GPIO_START 0x20200000
#define GPIO_END 0x2020002C

#define BIT_MASK(x, shift, mask) ((x >> shift) & mask)

#include <stdint.h>

typedef uint8_t byte;
typedef uint32_t word;

typedef struct state {
  byte memory[MEMSIZE];
  word registers[NOREGS];
  byte gpio_memory[GPIOSIZE];
  word gpio_clear;
  word gpio_set;
} state_t;

enum InstructionType { DP, M, SDT, B, T };

typedef struct instruction {
  enum InstructionType type;
  byte cond;
  union contents_u {
    struct dp_s { 
      byte i, opcode, s, rn, rd; 
      uint16_t operand2;  
    } dp;  
    struct m_s { 
      byte a, s, rd, rn, rs, rm; 
    } m;  
    struct sdt_s { 
      byte i, p, u, l, rn, rd; 
      uint16_t offset; 
    } sdt;  
    struct b_s { 
      word offset; 
    } b;  
  } contents;
} instruction_t;

/*
Usage:
word shifted_rm(uint16_t shift_rm, state_t* s);

Computes shifted register based on operand2 and offset fields in data processing and single data transfer instructions respectively
*/
word shifted_rm(uint16_t shift_rm, state_t* s);

/*
Usage:
word rotate_shift_right(word x, word shift_amount);

Computes the value of x rotated right by shift_amount bits
*/
word rotate_shift_right(word x, word shift_amount);

void pprint_instruction_t(instruction_t instruction);
void pprint_state_t(state_t s);

word get_word(byte *memory, int n);
word get_word_raw(byte *memory, int n);

void set_word(byte *memory, int addr, word w);

#endif
