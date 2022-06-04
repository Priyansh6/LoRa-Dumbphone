#include "utilities.h"
#include <stdint.h>


word shifted_rm(uint16_t shift_rm, state_t* s) {
  word rm = s->registers[shift_rm & 0xF];

  byte shift = (shift_rm >> 4) & 0xFF;
  byte shift_type = (shift >> 1) & 0x3;

  word shift_amount;
  if (shift & 1) {
    shift_amount = s->registers[shift >> 4];
  } else {
    shift_amount = shift >> 3;
  }

  /* Logical shift left */
  if (shift_type == 0) {
    return rm << shift_amount;
  }

  // Logical shift right /
  if (shift_type == 1) {
    return rm >> shift_amount; 
  }

  // Arithmetic shift right /
  if (shift_type == 2) {
    return (int32_t) rm >> shift_amount;
  }

  // Rotate shift right /
  shift_amount %= sizeof(word) ;
  return (rm >> shift_amount) | (rm << (sizeof(word) * 8 - shift_amount));
}

word imediate_offset(byte i, uint16_t offset_raw, state_t *state){

  if (i == 1) {
      return shifted_rm(offset_raw, state);
  } else {
      return offset_raw;
  }

}

word getWord(byte *memory, int n){
  int start = n ;

  return (memory[start] << 24)     |
         (memory[start + 1] << 16) |   
         (memory[start + 2] << 8)  |
         (memory[start + 3]);
}

void storeWord(word w, state_t *state, int dest){
  
  byte *bytes = (byte *) &w;
  
  state->memory[dest]     = bytes[0];
  state->memory[dest + 1] = bytes[1];
  state->memory[dest + 2] = bytes[2];
  state->memory[dest + 3] = bytes[3];

}

void execute_SDT(instruction_t inst, state_t *state){
  
  word rn = state->registers[inst.contents.sdt.i]; 
  word rd = state->registers[inst.contents.sdt.rd];

  word offset = imediate_offset(inst.contents.sdt.i, inst.contents.sdt.offset, state);
  
  int mult = (inst.contents.sdt.u == 0)? -1 : 1;
   
  int comp = rn + mult * offset;

  if (inst.contents.sdt.l == 0){
    storeWord(rd, state, comp);
  } else {
    state->registers[inst.contents.sdt.rd] = getWord(state->memory, comp);
  }

  if (inst.contents.sdt.p == 0 ){
    state->registers[inst.contents.sdt.rn] = comp;
  }

}

//int main(void){
  
//  state_t *s = malloc(sizeof(state_t));
  
  
  
  
//}


