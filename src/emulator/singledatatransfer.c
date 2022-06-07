#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "singledatatransfer.h"

void execute_SDT(instruction_t instruction, state_t *s) {
  
  word rn = s->registers[instruction.contents.sdt.rn]; 
  word rd = s->registers[instruction.contents.sdt.rd];
  
  word offset = instruction.contents.sdt.offset;

  offset = instruction.contents.sdt.i ? shifted_rm(offset, s) : offset;
  
  int multi = (instruction.contents.sdt.u == 0)? -1 : 1;
   
  word addr = rn;
  
  if (instruction.contents.sdt.p) {
    addr += multi * offset;
  }
 
  if ((addr >= MEMSIZE && addr < GPIO_START) || addr >= GPIO_END) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", addr);
    return;
  }
 
  if (instruction.contents.sdt.l == 0) {
    if (addr >= GPIO_START) {
      switch (addr) {
        case GPIO_CLEAR:
          clear_pin(s, rd);
          break;
        case GPIO_SET:
          set_pin(s, rd);
          break;
        default:
          configure_pin(s, addr, rd);
      }
    } else {
      set_word(s->memory, addr, rd);
    }
  } else {
    if (addr >= GPIO_START) {
      s->registers[instruction.contents.sdt.rd] = addr;
      pprint_access_message(addr);
    } else {
      s->registers[instruction.contents.sdt.rd] = get_word(s->memory, addr);
    }
  }

  if (instruction.contents.sdt.p == 0) {
    s->registers[instruction.contents.sdt.rn] = addr + multi * offset;
  }
}
