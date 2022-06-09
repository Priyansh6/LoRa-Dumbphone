#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

// START  MACROS ------------------------------------------------------------
#define INCREMENT_IF_STARTS_WITH(chr, val) if (chr[0] == val) {chr++;}
#define SKIP_CHARS(chr, val) chr += val;

#define FUNCTION_START(name, code)\
void name(token_t *token, char *str_pointer, char *sub_token){\
  token->format = code;

#define SET_IN_TOKEN_LAST(space_name, struct_name, convertion_function)\
if(strlen(str_pointer) == 0){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = convertion_function(str_pointer); \
} 

#define SET_IN_TOKEN(space_name, struct_name, convertion_function)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = convertion_function(sub_token); \
}

#define GET_NEXT_TOKEN sub_token = strtok_r(NULL, ",", &str_pointer);

#define  GOTO_TYPE_FUNC(str_case ,dest)\
if (strcmp(str_case, sub_token) == 0) { \
  \
  dest(&token, str_pointer, sub_token);\
  return token;\
}

// END MACROS ------------------------------------------------------------

word toimmediate(char *str) {
  str++;
  return strlen(str) >= 2 && str[1] == 'x' ? strtoul(str + 2, NULL, 16) : strtoul(str, NULL, 10);
}

byte get_reg_number(char *reg){
  INCREMENT_IF_STARTS_WITH(reg, 'r')
  return strtoul(reg, NULL, 10);
}



void get_shift(shift_t *shift, char *str_holder){
  char *opr;
  printf("here\n");

  size_t line_length = strlen(str_holder);

  //if (line_length  == 0 || line_length == 1 )

  opr = malloc(line_length + 1);
  char *free_p = opr;
  strncpy(opr, str_holder, line_length);
  
  if (opr[0] == 'r') {
    printf("here\n");

    shift->i = false; 
    char *rest;
    SKIP_CHARS(opr, 1)
    printf("here\n");
    shift->values_oper_t.sh_reg.rm = strtoul(opr, &rest, 10);
    printf("here\n");

    if (strlen(rest) > 3) {
      printf("2 %s\n", rest);
      INCREMENT_IF_STARTS_WITH(rest, ',')
      INCREMENT_IF_STARTS_WITH(rest, ' ')
      printf("2 %s\n", rest);

      if (!strncmp(rest, "lsl", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 0;
      }
      else if (!strncmp(rest, "lsr", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 1;
      }
      else if (!strncmp(rest, "asr", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 2;
      }
      else if (!strncmp(rest, "ror", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 3;
      }
      printf("2\n");

      SKIP_CHARS(rest, 4)
      printf("2\n");

      if (rest[0] == 'r') {
        printf("2\n");
        SKIP_CHARS(rest, 1)
        printf("2\n");
        shift->values_oper_t.sh_reg.is_reg = true;
        shift->values_oper_t.sh_reg.shifted_vals_t.rs = strtoul(rest, NULL, 10);
        printf("2 %s\n", rest);
      } else {
        shift->values_oper_t.sh_reg.is_reg = false;
        shift->values_oper_t.sh_reg.shifted_vals_t.immediate = toimmediate(rest);
      }
    } else {
      shift->i = true;
      shift->values_oper_t.immediate = 0;
    }
  } else {
    shift->i = true;
    shift->values_oper_t.immediate = toimmediate(opr);
  }

  free(free_p);

}

void get_addr(address_s_t *addr, char *str_holder){
  char *add;
  printf("here\n");

  size_t line_length = strlen(str_holder);
  add = malloc(line_length + 1);
  char *free_p = add;
  strncpy(add, str_holder, line_length);

  if (add[0] == '='){
    addr->i = true;
    addr->values_addr_t.immediate = toimmediate(add);
  } else {
    char *rest;

    addr->i = false;
    INCREMENT_IF_STARTS_WITH(add, '[')

    INCREMENT_IF_STARTS_WITH(add, 'r')
    printf("%s\n", add);
    addr->rn = strtoul(add, &rest, 10);

    if (rest[0] == ']') {
      addr->p = true; 
      if (strlen(rest) == 1 ){
        addr->values_addr_t.shift.i = true; 
        addr->values_addr_t.shift.values_oper_t.immediate = 0; 
      } else {
        INCREMENT_IF_STARTS_WITH(rest, ']')
        INCREMENT_IF_STARTS_WITH(rest, ',')
        INCREMENT_IF_STARTS_WITH(rest, ' ')
        printf("%s\n", rest);

        if (rest[0] == '-'){
          addr->sighn = false;
        } else {
          addr->sighn = true;
        }
        INCREMENT_IF_STARTS_WITH(rest, '+')
        INCREMENT_IF_STARTS_WITH(rest, '-')
        printf("%s\n", rest);

        get_shift(&addr->values_addr_t.shift, rest);
      }
    } else {
      printf("%s\n", rest);
      INCREMENT_IF_STARTS_WITH(rest, ',')
      INCREMENT_IF_STARTS_WITH(rest, ' ')
      
      if (rest[0] == '-'){
          addr->sighn = false;
      } else {
          addr->sighn = true;
      }
      printf("%s\n", rest);
      INCREMENT_IF_STARTS_WITH(rest, '+')
      INCREMENT_IF_STARTS_WITH(rest, '-')

      get_shift(&addr->values_addr_t.shift, rest);
    }
  }
}

bool isnumber(char *str, byte base) {
  char max_digit = base <= 10 ? '0' + base - 1 : 'A' + base - 11;
  for (char *x = str; *x; x++) {
    char upper = isalpha(*x) ? toupper(*x) : *x;
    if (upper < '0' || upper > max_digit || (upper > '9' && upper < 'A')) {
      return false;
    }
  }
  return true;
}




FUNCTION_START(tokenise_comp ,DP_COMP_F)
  SET_IN_TOKEN(opcode, dp_comp_f, )
  GET_NEXT_TOKEN 
  SET_IN_TOKEN(rd, dp_comp_f, get_reg_number)
  GET_NEXT_TOKEN 
  SET_IN_TOKEN(rn, dp_comp_f, get_reg_number)

  if(strlen(str_pointer) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_comp_f.operand2 , str_pointer);
  }

}

FUNCTION_START(tokenise_mov, DP_MOV_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, dp_mov_f, get_reg_number)

  if(strlen(str_pointer) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_mov_f.operand2 , str_pointer);
  }

}

FUNCTION_START(tokenise_ncomp, DP_NCOMP_F)
  SET_IN_TOKEN(opcode, dp_ncomp_f, )
  GET_NEXT_TOKEN  
  SET_IN_TOKEN(rn, dp_ncomp_f, get_reg_number)
  if(strlen(str_pointer) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_comp_f.operand2 , str_pointer);
  }
}

FUNCTION_START(tokenise_m , M_F)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, m_f, get_reg_number)
  GET_NEXT_TOKEN  
  SET_IN_TOKEN(rm, m_f, get_reg_number)
  SET_IN_TOKEN_LAST(rs, m_f, get_reg_number)
}

FUNCTION_START(tokenise_ma, MA_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, ma_f, get_reg_number)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rm, ma_f, get_reg_number)
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rs, ma_f, get_reg_number)
  SET_IN_TOKEN_LAST(rn, ma_f, get_reg_number)
}

FUNCTION_START(tokenise_sdt, SDT_F)
  SET_IN_TOKEN(expr, sdt_f, )
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rd, sdt_f, get_reg_number)
  if(strlen(str_pointer) == 0){
    token->format = INV_F;
    return;
  } else {
    get_addr(&token->contents_f.sdt_f.addr , str_pointer);
  }
}


FUNCTION_START(tokenise_b, B_F)
  SET_IN_TOKEN(cond, b_f, )
  SET_IN_TOKEN_LAST(offset, b_f, )
}

FUNCTION_START(tokenise_andeq, ANDEQ_F)
}

FUNCTION_START(tokenise_lsl, LSL_F) 
  GET_NEXT_TOKEN
  SET_IN_TOKEN(rn, lsl_f, get_reg_number)
  SET_IN_TOKEN_LAST(expr, lsl_f, )
}

token_t tokenise_line(const char *line){
  token_t token; 

  size_t line_length = strlen(line);
 
  char *str_holder, *str_pointer, *sub_token;
  str_holder = malloc(line_length + 1);
  strncpy(str_holder, line, line_length);
  //token.free_pointer = str_holder; 
 
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



int main(void){
  //= (operand2_t *) malloc(sizeof(operand2_t));
  token_t t = tokenise_line("sub r1,r1,#1");
  //printf("1\n");
  //printf("4 %x\n", op.values_oper_t.sh_reg.shifted_vals_t.immediate);

  printf("opcode %s\n", t.contents_f.dp_comp_f.opcode);
  printf("rd %x\n", t.contents_f.dp_comp_f.rd);
  printf("rn %x\n", t.contents_f.dp_comp_f.rn);
  printf("operand2-i %x\n", t.contents_f.dp_comp_f.operand2.i);
  printf("operand2-immediate%x\n", t.contents_f.dp_comp_f.operand2.values_oper_t.immediate);




return 0;
}

