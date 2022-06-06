#ifndef GPIO_H
#define GPIO_H

#define PIN_INPUT 0
#define PIN_OUTPUT 1

#include "utilities.h"

void pprint_access_message(word addr);

void clear_pin(state_t *s, word w);
void set_pin(state_t *s, word w);
void configure_pin(state_t *s, word address, word w);

#endif
