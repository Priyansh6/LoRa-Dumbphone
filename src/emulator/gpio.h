#ifndef GPIO_H
#define GPIO_H

#define PIN_INPUT 0
#define PIN_OUTPUT 1

#include "../utilities.h"

/*
Usage:
pprint_access_message(word addr)

Prints a message everytime a GPIO pin is accessed and specifices which pin was accessed.
*/
void pprint_access_message(word addr);

/*
Usage:
clear_pin(state_t *s, word w)

Clears a specified GPIO pin.
*/
void clear_pin(state_t *s, word w);

/*
Usage:
set_pin(state_t *s, word w)

Sets a specified GPIO pin.
*/
void set_pin(state_t *s, word w);

/*
Usage:
configure_pin(state_t *s, word address, word w)

Configures the state of a pin to either input or output.
*/
void configure_pin(state_t *s, word address, word w);

#endif
