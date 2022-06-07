#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "utilities.h"


int write_binary(const char *fname, const word *buff, size_t no_words) {

  byte *buff_bytes = (byte *) buff;

  FILE *fp;
  fp = fopen(fname, "w");
  if (fp == NULL){
    perror("error opening file!");
    exit(EXIT_FAILURE);
  }
  
  fwrite(buff_bytes, sizeof(byte), no_words * 4, fp);


  fclose(fp);
}

int main(void){
  
  char *fname = "test"; 
  
  word* numbers = malloc(sizeof(word) * 40);

  for (int i = 0 ; i < 40; i++){
    numbers[i] = i;
  }

  write_binary(fname, numbers, 40);

  free(numbers);

  return 0; 

}

