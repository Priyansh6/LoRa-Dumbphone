#include "utilities.h"

#include <stdio.h>

void clear_pin(state_t *s, word w) {
  s->gpio_clear = w;
  printf("PIN OFF\n");
}

void set_pin(state_t *s, word w) {
  s->gpio_set = w;
  printf("PIN ON\n");
}

void pprint_access_message(word addr) {
  switch (addr - GPIO_START) {
    case 0:
      printf("One GPIO pin from 0 to 9 has been accessed\n");
      break;
    case 4:
      printf("One GPIO pin from 10 to 19 has been accessed\n");
      break;
    case 8:
      printf("One GPIO pin from 20 to 29 has been accessed\n");
      break;
  }
}

void configure_pin(state_t *s, word addr, word w) {
  set_word(s->gpio_memory, addr - GPIO_START, w);
  pprint_access_message(addr);
}

