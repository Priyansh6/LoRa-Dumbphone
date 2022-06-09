#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

#define FUNCTION_START(name, code)\
void name(token_t *token, char *str_pointer, char *sub_token){\
  token->format = code;

#define SET_IN_TOKEN_LAST(space_name, struct_name)\
if(strlen(str_pointer) == 0){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = str_pointer; \
} 

#define SET_IN_TOKEN(space_name, struct_name)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = sub_token; \
}

#define GET_NEXT_TOKEN sub_token = strtok_r(NULL, ",", &str_pointer);

#define  GOTO_TYPE_FUNC(str_case ,dest)\
if (strcmp(str_case, sub_token) == 0) { \
  \
  dest(&token, str_pointer, sub_token);\
  return token;\
}

FUNCTION_START(tokenise_comp ,DP_COMP_F)
  SET_IN_TOKEN(opcode, dp_comp_f)
  GET_NEXT_TOKEN 
  SET_IN_TOKEN(rd, dp_comp_f)
  GET_NEXT_TOKEN 
  SET_IN_TOKEN(rn, dp_comp_f)
  SET_IN_TOKEN_LAST(operand2, dp_comp_f)
}

FUNCTION_START(tokenise_mov, DP_MOV_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, dp_mov_f)
  SET_IN_TOKEN_LAST(operand2, dp_mov_f)
}

FUNCTION_START(tokenise_ncomp, DP_NCOMP_F )
  SET_IN_TOKEN(opcode, dp_ncomp_f)
  GET_NEXT_TOKEN  
  SET_IN_TOKEN(rn, dp_ncomp_f)
  SET_IN_TOKEN_LAST(operand2, dp_ncomp_f)
}

FUNCTION_START(tokenise_m , M_F)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, m_f)
  GET_NEXT_TOKEN  
  SET_IN_TOKEN(rm, m_f)
  SET_IN_TOKEN_LAST(rs, m_f)
}

FUNCTION_START(tokenise_ma, MA_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, ma_f)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rm, ma_f)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rs, ma_f)
  SET_IN_TOKEN_LAST(rn, ma_f)
}

FUNCTION_START(tokenise_sdt, SDT_F)
  SET_IN_TOKEN(expr, sdt_f)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, sdt_f)
  SET_IN_TOKEN_LAST(addr, sdt_f)
}


FUNCTION_START(tokenise_b, B_F)
  SET_IN_TOKEN(cond, b_f)
  SET_IN_TOKEN_LAST(offset, b_f)
}

FUNCTION_START(tokenise_andeq, ANDEQ_F)
}

FUNCTION_START(tokenise_lsl, LSL_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rn, lsl_f)
  SET_IN_TOKEN_LAST(expr, lsl_f)
}

token_t tokenise_line(const char *line){
  token_t token; 

  size_t line_length = strlen(line);
 
  char *str_holder, *str_pointer, *sub_token;
  str_holder = malloc(line_length + 1);
  strncpy(str_holder, line, line_length);
  token.free_pointer = str_holder; 
 
  sub_token = strtok_r(str_holder, " ", &str_pointer);

  GOTO_TYPE_FUNC("and", tokenise_comp )
  GOTO_TYPE_FUNC("eor", tokenise_comp )
  GOTO_TYPE_FUNC("sub", tokenise_comp )
  GOTO_TYPE_FUNC("rsb", tokenise_comp )
  GOTO_TYPE_FUNC("add", tokenise_comp )
  GOTO_TYPE_FUNC("orr", tokenise_comp )

  GOTO_TYPE_FUNC("mov", tokenise_mov )

  GOTO_TYPE_FUNC("tst", tokenise_ncomp )
  GOTO_TYPE_FUNC("teq", tokenise_ncomp )
  GOTO_TYPE_FUNC("cmp", tokenise_ncomp )

  GOTO_TYPE_FUNC("mul", tokenise_m)
  GOTO_TYPE_FUNC("mla", tokenise_ma )

  GOTO_TYPE_FUNC("ldr", tokenise_sdt )
  GOTO_TYPE_FUNC("str", tokenise_sdt )

  GOTO_TYPE_FUNC("andeq", tokenise_andeq )
  GOTO_TYPE_FUNC("lsl", tokenise_lsl )

  if (strncmp(sub_token, "b", 1)){
    tokenise_b(&token, str_pointer, sub_token);
    return token; 
  }

  token.format = INV_F;
  return token;
  
}


/*
int main(void){

  token_t t = tokenise_line("ldr r3,[r1,#0x4]");
 
  printf("1 %s\n", t.contents_f.sdt_f.expr);
  printf("2 %s\n", t.contents_f.sdt_f.rd);
  printf("3 %s\n", t.contents_f.sdt_f.addr);

  free(t.free_pointer);

return 0;
}
*/
