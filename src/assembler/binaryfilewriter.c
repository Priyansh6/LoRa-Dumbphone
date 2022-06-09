#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../utilities.h"

word endian_swap(word w) {
  return (w & 0xFF)              << 24 | 
         (BIT_MASK(w, 8, 0xFF))  << 16 | 
         (BIT_MASK(w, 16, 0xFF)) << 8  |
         (BIT_MASK(w, 24, 0xFF));
}


void write_binary(const char *fname, const word *buff, size_t no_words) {
 
  FILE *fp;
  fp = fopen(fname, "w");
  if (fp == NULL){
    perror("error opening file!");
    exit(EXIT_FAILURE);
  }
  
  //word swapped;
  for (int i = 0; i < no_words; i++){
    fwrite(&buff[i], sizeof(word), 1, fp);
  }

  fclose(fp);

}
