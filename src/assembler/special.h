#ifndef SPECIAL_H
#define SPECIAL_H

#include "../utilities.h"

/*
USAGE 
assemble_ANDEQ(token_t t);

This function is tasked with populating a word with the format specified
by a terminate/aneeq instruction.
*/
word assemble_ANDEQ(token_t t); 

/*
USAGE 
assemble_LSL(token_t t);

This function is tasked with populating a word with the format specified
by a logical shift left instruction.
*/
word assemble_LSL(token_t t);

#endif
