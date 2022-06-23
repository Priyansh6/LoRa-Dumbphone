#include <stdio.h>
#include <stdlib.h>

#include "../utilities.h"

int write_binary(const char *fname, const word *buff, size_t no_words) {
  FILE *fp;
  fp = fopen(fname, "w");
  if (fp == NULL) {
    perror("error opening file!");
    exit(EXIT_FAILURE);
  }
  
  for (int i = 0; i < no_words; i++) {
    fwrite(&buff[i], sizeof(word), 1, fp);
  }

  fclose(fp);
  return 0;
}
