#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilities.h"
#include "assembler/symboltable.h"


int main(int argc, char **argv) {

  char *filename = argv[1];

  FILE *fp;
  fp = fopen(filename, "r");

  symbol_table_t st;
  init_symbol_table(&st, get_label, add_label);

  char line[256];
  address count = 0;

  //first pass
  while (fgets(line, sizeof(line), fp)){

    size_t line_length = strlen(line);
    //get reid of new lines
    if (line[line_length - 1] == '\n'){
       line[line_length - 1] = '\0';
       line_length--;
    }
    //check if line is label last charecter is :
    if (line[line_length - 1] == ':'){
      line[line_length - 1] = '\0';
      st.add(&st, line, count + 4);
      printf("label %s\n", line);
    }
    count += 4;
  }

  pprint_symbol_table(st);
  
 return EXIT_SUCCESS;
}
