#ifndef UTILITIES_H
#define UTILITIES_H

#define MEMSIZE (64 * 1024)
#define NOREGS 17
#define PC 15
#define CPSR 16
#define GPIOSIZE 12
#define GPIO_CLEAR 0x20200028
#define GPIO_SET 0x2020001C
#define GPIO_START 0x20200000
#define GPIO_END 0x2020002C

#define BIT_MASK(x, shift, mask) ((x >> shift) & mask)
#define BOOL_STR(b) (b ? "true" : "false")

#include <stdbool.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t byte;
typedef uint16_t address;
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

enum InstructionFormat {DP_COMP_F, DP_MOV_F, DP_NCOMP_F, M_F, MA_F, SDT_F, B_F, ANDEQ_F, LSL_F, INV_F};

typedef struct shifted_reg {
  bool is_reg;
  byte shift_type;
  byte rm;
  union shifted_vals {
    byte immediate, rs;
  } shifted_vals_t;
} shifted_reg_t;

typedef struct shift {
  bool i;
  union values_oper {
    byte immediate;
    shifted_reg_t sh_reg;   
  } values_oper_t;
} shift_t;

typedef struct address_s {
  bool p;
  bool i;
  bool sighn;
  byte rn; 
  union values_addr{
    word immediate;
    shift_t shift;
  } values_addr_t;
} address_s_t ;

typedef struct token {
  enum InstructionFormat format;
  union contents_f_u {
    struct dp_comp_f_s {
      char *opcode;
      int rd, rn; 
      shift_t operand2;
    } dp_comp_f;
    struct dp_mov_f_s {
      int rd;
      shift_t operand2;
    } dp_mov_f;
    struct dp_ncomp_f_s {
      char *opcode; 
      int rn; 
      shift_t operand2;
    } dp_ncomp_f;
    struct m_f_s {
      int rd, rm, rs;
    } m_f;
    struct ma_f_s {
      int rd, rm, rs, rn; 
    } ma_f;
    struct sdt_f_s {
      char *expr; 
      int rd; 
      address_s_t addr;
    } sdt_f;
    struct b_f_s {
      char *cond, *offset;
    } b_f;
    struct lsl_f_s {
      int rn; 
      byte expr;
    } lsl_f;
  } contents_f;
} token_t;

void pprint_shifted_reg(shifted_reg_t srt);
void pprint_shift(shift_t srt);
void pprint_addr(address_s_t srt);

void pprint_SDT(token_t token);
void pprint_token(token_t t);
/*
Usage: 
bool isnumber(char *str);

Returns whether a string is a number in the provided base
Works up to base 36
*/
//bool isnumber(char *str, byte base);

/*
Usage:
word toimmediate(char *str);

Returns number converted from immediate constant token in assembly instruction
Assumes first character is a '#' or '=' followed by an immediate constant in base 10 or 16
*/
//word toimmediate(char *str);

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


/*
Usage:
pprint_instruction_t(instruction_t instruction);

Pretty prints the instruction type with all fields. Useful for debugging.
*/
void pprint_instruction_t(instruction_t instruction);

/*
Usage:
pprint_state_t(state_t s);

Pretty prints the state. Outputs all registers and non-zero memory.
*/
void pprint_state_t(state_t s);

/*
Usage:
get_word(byte *memory, int n);

Gets a word from memory[n] and puts in in big endian form for processing.
*/
word get_word(byte *memory, int n);

/*
Usage:
get_word_raw(byte *memory, int n);

Gets a word from memory[n] as it is laid out in memory.
*/
word get_word_raw(byte *memory, int n);

/*
Usage:
set_word(byte *memory, int addr, word w);

Sets the next four bytes in memory from addr to the four bytes comprising word.
*/
void set_word(byte *memory, int addr, word w);

#endif
