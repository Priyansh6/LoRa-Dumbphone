#ifndef TOKENISER_H
#define TOKENISER_H

#include "../utilities.h"

/*
USAGE 
tokenise_line(token_t token, const char *line)

This function is tasked in populating the token t with the data specifed in the string line.
It also is tasked in converting types.
*/
void tokenise_line(token_t *token, const char *line);

#endif
