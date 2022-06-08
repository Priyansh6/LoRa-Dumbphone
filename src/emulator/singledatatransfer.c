#include <stdint.h>
#include <stdio.h>

#include "gpio.h"
#include "singledatatransfer.h"

#define INSSDT instruction.contents.sdt

void execute_SDT(instruction_t instruction, state_t *s) {
  
  word rn = s->registers[INSSDT.rn]; 
  word rd = s->registers[INSSDT.rd];
  
  word offset = INSSDT.offset;

  offset = INSSDT.i ? shifted_rm(offset, s) : offset;
  
  int multi = (INSSDT.u == 0)? -1 : 1;
   
  word addr = rn;
  
  if (INSSDT.p) {
    addr += multi * offset;
  }
 
  if ((addr >= MEMSIZE && addr < GPIO_START) || addr >= GPIO_END) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", addr);
    return;
  }
 
  if (INSSDT.l == 0) {
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
      s->registers[INSSDT.rd] = addr;
      pprint_access_message(addr);
    } else {
      s->registers[INSSDT.rd] = get_word(s->memory, addr);
    }
  }

  if (INSSDT.p == 0) {
    s->registers[INSSDT.rn] = addr + multi * offset;
  }
}
