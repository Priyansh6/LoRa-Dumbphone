#ifndef TOKENISER_H
#define TOKENISER_H

#include "../utilities.h"

/*
USAGE 
tokenise_line(token_t token, const char *line)

this function is tasked in populating the token t with the data spesifed in the string line.
it also is tasked in convetring types aswell.
*/
void tokenise_line(token_t *token, const char *line);

#endif
