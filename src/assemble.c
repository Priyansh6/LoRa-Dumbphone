#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assembler/binaryfilewriter.h"
#include "assembler/branch.h"
#include "assembler/dataprocessing.h"
#include "assembler/multiply.h"
#include "assembler/singledatatransfer.h"
#include "assembler/special.h"
#include "assembler/stack.h"
#include "assembler/symboltable.h"
#include "assembler/tokeniser.h"
#include "utilities.h"

#define DEBUG_STATEMENT(x) if(is_debug) {x;};

bool is_debug = true;

int main(int argc, char **argv) {

  char *filename = argv[1];

  FILE *fp;
  fp = fopen(filename, "r");
  if(fp == NULL){
    perror("error opening file!");
    exit(EXIT_FAILURE);
  }

  symbol_table_t st;
  init_symbol_table(&st, get_label, add_label);

  char line[256];
  address count = 0;

  // First pass
  while (fgets(line, sizeof(line), fp)){

    size_t line_length = strlen(line);
    // Get rid of new lines
    if (line[line_length - 1] == '\n'){
       line[line_length - 1] = '\0';
       line_length--;
    }
    // Check if line is label (last charecter is ':')
    if (line[line_length - 1] == ':'){
      line[line_length - 1] = '\0';
      st.add(&st, line, count);
      DEBUG_STATEMENT(printf("label %s\n", line))
    } else if (line_length > 1){
      count += 4;
    }
  }

  // Second pass 
  rewind(fp);
  
  token_t t = { .format = 0 };  
  word w;  
  address inst_count = 0;
  word *outBuff = (word *) calloc(1, (MEMSIZE/4));

  stack_t *constants_stack = alloc_stack();

  while (fgets(line, sizeof(line), fp)){
    size_t line_length = strlen(line);
    // Get rid of new lines
    if (line[line_length - 1] == '\n'){
       line[line_length - 1] = '\0';
       line_length--;
    }

    if (line_length <= 1 ){
      continue;
    }

    DEBUG_STATEMENT(printf("CONVERTING %s\n", line))
    tokenise_line(&t, line);
    DEBUG_STATEMENT(pprint_token(t))

    switch (t.format) {
      case DP_COMP_F: 
      case DP_MOV_F: 
      case DP_NCOMP_F:
        w = assemble_DP(t);
        inst_count += 1;
        break;
      case M_F: 
      case MA_F:
        w = assemble_M(t);
        inst_count += 1;
        break;
      case SDT_F:
        w = assemble_SDT(t, inst_count * 4, count/4, constants_stack);
        inst_count += 1;
        break;
      case B_F:
        w = assemble_B(t, &st, inst_count * 4); 
        inst_count += 1;
        break;
      case ANDEQ_F:
        w = assemble_ANDEQ(t);
        inst_count += 1;
        break;
      case LSL_F:
        w = assemble_LSL(t);
        inst_count += 1;
        break;
      case INV_F: 
      default:
        DEBUG_STATEMENT(printf("PROBABLY LABEL\n"))
        break;
    }

    DEBUG_STATEMENT(printf("0x%x %x\n", inst_count, w);
    printf("\n\n"))

    outBuff[inst_count - 1] = w;
  }

  DEBUG_STATEMENT(pprint_stack(constants_stack));

  // Adding the constants to the outBuff
  int number_constants = size(constants_stack);

  for (int i = number_constants - 1 ; i >= 0 ; i--){
    word constx = pop(constants_stack);
    DEBUG_STATEMENT(printf("CONSANT POPPED %x\n", constx);
    printf("ADDING AT %x\n", count + (i * 4)))
    outBuff[(count + (i * 4))/ 4] = constx;
  }

  free_stack(constants_stack);
  free_symbol_table(&st);
  fclose(fp);

  write_binary(argv[2], outBuff, (inst_count + number_constants));
  free(outBuff);

  return EXIT_SUCCESS;
}


