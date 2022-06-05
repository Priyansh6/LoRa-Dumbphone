#include "utilities.h"

void clear_pin(state_t *s, word w) {
  s->gpio_clear = w;
}

void set_pin(state_t *s, word w) {
  s->gpio_set = w;
}

void configure_pin(state_t *s, word addr, word w) {
   set_word(s->gpio_memory, addr - GPIO_START, w);
}

