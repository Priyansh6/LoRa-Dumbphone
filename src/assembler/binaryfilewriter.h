#ifndef BINARYFILEWRITER_H
#define BINARYFILEWRITER_H

#include "../utilities.h"

/*
Usage:
write_binary(const char *fname, const word *buff, size_t no_words);

Writes the no_words number of words in the array buff into a file named fname. 
If the file does not exist it will create a new file
*/
int write_binary(const char *fname, const word *buff, size_t no_words);

#endif
