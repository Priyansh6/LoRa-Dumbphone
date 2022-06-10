#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilities.h"
#include "assembler/symboltable.h"
#include "assembler/tokeniser.h"

#include "assembler/special.h"
#include "assembler/multiply.h"
#include "assembler/singledatatransfer.h"
#include "assembler/dataprocessing.h"
#include "assembler/branch.h"
#include "assembler/binaryfilewriter.h"
#include "assembler/stack.h"

#define DEBUG(x) if(is_debug) {x;};

bool is_debug = true;

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
      st.add(&st, line, count);
      DEBUG(printf("label %s\n", line))
    } else if (line_length > 1){
      count += 4;
    }
  }

  //pprint_symbol_table(st);
  //second pass 

  rewind(fp);
  
  
  token_t t = { .format = 0 };  
  word w;  
  address inst_count = 0;
  word *outBuff = (word *) calloc(1, (MEMSIZE/4));

  stack_t *constants_stack = alloc_stack();


  while (fgets(line, sizeof(line), fp)){

    size_t line_length = strlen(line);
    //get reid of new lines
    if (line[line_length - 1] == '\n'){
       line[line_length - 1] = '\0';
       line_length--;
    }

    if (line_length <= 1 ){
      continue;
    }

    DEBUG(printf("CONVERTING %s\n", line))
    tokenise_line(&t, line);
    DEBUG(pprint_token(t))



    switch (t.format) {
      case DP_COMP_F: 
      case DP_MOV_F: 
      case DP_NCOMP_F:
        w = assemble_DP(t);
        printf("w: %x\n", w);

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
        DEBUG(printf("PROB LABEL\n"));
        break;
    }

    DEBUG(printf("0x%x %x\n", inst_count, w);
    printf("\n\n"))

    outBuff[inst_count - 1] = w; 
 
  }

    pprint_stack(constants_stack);

    //adding the constants to the outBuff 

    int number_constants = size(constants_stack);

    for (int i = number_constants - 1 ; i >= 0 ; i--){
      word constx = pop(constants_stack);
      printf("CONSANT POPED %x\n", constx);
      printf("ADDING AT %x\n", count + (i * 4) );
      outBuff[(count + (i * 4))/ 4] = constx;
    }

    write_binary(argv[2], outBuff, (inst_count + number_constants));
    free_stack(constants_stack);
  
 return EXIT_SUCCESS;
}
