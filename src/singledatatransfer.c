#include "utilities.h"
#include <stdint.h>

word imediate_offset(byte i, uint16_t offset_raw, state_t *state){

  if (i == 1) {
      return shifted_rm(offset_raw, state);
  } else {
      return offset_raw;
  }

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

  printf("%d\n", comp);

  if (inst.contents.sdt.l == 0){
    storeWord(rd, state, comp);
  } else {
    state->registers[inst.contents.sdt.rd] = get_word(state->memory, comp);
  }

  if (inst.contents.sdt.p == 0 ){
    state->registers[inst.contents.sdt.rn] = comp;
  }

}

//int main(void){
  
//  state_t *s = malloc(sizeof(state_t));
  
  
  
  
//}


