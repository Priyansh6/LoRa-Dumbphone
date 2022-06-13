#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

// START  MACROS ------------------------------------------------------------
#define INCREMENT_IF_STARTS_WITH(chr, val) while (1) { if (chr[0] == val) {chr++;} else {break;} } 
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

}

// END MACROS ------------------------------------------------------------

word toimmediate(char *str) {
  INCREMENT_IF_STARTS_WITH(str, ' ')
  str++;
  return strlen(str) >= 2 && str[1] == 'x' ? strtoul(str + 2, NULL, 16) : strtoul(str, NULL, 10);
}

byte get_reg_number(char *reg){
  INCREMENT_IF_STARTS_WITH(reg, 'r')
  return strtoul(reg, NULL, 10);
}



void get_shift(shift_t *shift, char *str_holder){
  char *opr;

  printf("%s\n", str_holder);
  size_t line_length = strlen(str_holder);

  if (line_length  == 0 || line_length == 1 ) {
    shift->i = false;
    shift->values_oper_t.sh_reg.is_reg = false; 
    shift->values_oper_t.sh_reg.shifted_vals_t.immediate = 0;
    return;
  }

  opr = malloc(line_length + 1);
  char *free_p = opr;
  strncpy(opr, str_holder, line_length);

  INCREMENT_IF_STARTS_WITH(opr, ' ')

  
  if (opr[0] == 'r') {

    shift->i = false; 
    char *rest;
    SKIP_CHARS(opr, 1)
    shift->values_oper_t.sh_reg.rm = strtoul(opr, &rest, 10);

    if (strlen(rest) > 3) {
      INCREMENT_IF_STARTS_WITH(rest, ',')
      INCREMENT_IF_STARTS_WITH(rest, ' ')


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

      SKIP_CHARS(rest, 4)

      if (rest[0] == 'r') {
        SKIP_CHARS(rest, 1)
        shift->values_oper_t.sh_reg.is_reg = true;
        shift->values_oper_t.sh_reg.shifted_vals_t.rs = strtoul(rest, NULL, 10);
      } else {
        shift->values_oper_t.sh_reg.is_reg = false;
        shift->values_oper_t.sh_reg.shifted_vals_t.immediate = toimmediate(rest);
      }
    } else {
      shift->values_oper_t.sh_reg.shift_type = 0;
      shift->values_oper_t.sh_reg.is_reg = false;
      shift->values_oper_t.sh_reg.shifted_vals_t.immediate = 0;
    }
  } else  {
    shift->i = true;
    shift->values_oper_t.immediate = toimmediate(opr);
  }

  free(free_p);

}

void get_addr(address_s_t *addr, char *str_holder){
  char *add;
  
  size_t line_length = strlen(str_holder);
  add = malloc(line_length + 1);
  char *free_p = add;
  strncpy(add, str_holder, line_length);
  INCREMENT_IF_STARTS_WITH(add, ' ')


  addr->sighn = true;

  if (add[0] == '='){
    addr->i = true;
    addr->values_addr_t.immediate = toimmediate(add);
  } else {
    char *rest;

    addr->i = false;
    INCREMENT_IF_STARTS_WITH(add, '[')
    INCREMENT_IF_STARTS_WITH(add, 'r')
    addr->rn = strtoul(add, &rest, 10);

    if (rest[0] == ']') {
      addr->p = false; 
      if (strlen(rest) == 1 ){
        addr->p = true; 
        addr->values_addr_t.shift.i = true; 
        addr->values_addr_t.shift.values_oper_t.immediate = 0; 
      } else {
        INCREMENT_IF_STARTS_WITH(rest, ']')
        INCREMENT_IF_STARTS_WITH(rest, ',')
        INCREMENT_IF_STARTS_WITH(rest, ' ')

        printf("Checking signh of the value %s\n", rest);


        if (rest[0] == '-'){
          addr->sighn = false;
        } else if (rest[1] == '-') {
          addr->sighn = false;
          SKIP_CHARS(rest, 1);
        } else {
          addr->sighn = true;
        }

        printf("Checking signh of the value %s\n", rest);


        INCREMENT_IF_STARTS_WITH(rest, '+')
        INCREMENT_IF_STARTS_WITH(rest, '-')

        get_shift(&addr->values_addr_t.shift, rest);
      }
    } else {
      addr->p = true; 

      INCREMENT_IF_STARTS_WITH(rest, ',')
      INCREMENT_IF_STARTS_WITH(rest, ' ')

      printf("Checking signh of the value %s\n", rest);
      
      if (rest[0] == '-'){
          addr->sighn = false;
      } else if (rest[1] == '-') {
          addr->sighn = false;
          rest[1] = '#';
          SKIP_CHARS(rest, 1)
      } else {
          addr->sighn = true;
      }
      
      printf("Checking signh of the value %s\n", rest);

      INCREMENT_IF_STARTS_WITH(rest, '+')
      INCREMENT_IF_STARTS_WITH(rest, '-')

      get_shift(&addr->values_addr_t.shift, rest);
    }
  }

  free(free_p);

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
    get_shift(&token->contents_f.dp_ncomp_f.operand2 , str_pointer);
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
  SET_IN_TOKEN_LAST(expr, lsl_f, toimmediate)
}

void tokenise_line(token_t *token, const char *line){

  size_t line_length = strlen(line);
 
  char *str_pointer, *sub_token;
  char str_holder[line_length + 1];
  //str_holder = (char *) malloc(line_length + 1);
  // tnaeounh
  strncpy(str_holder, line, line_length);
  //token->free_pointer = str_holder; 
 
  sub_token = strtok_r(str_holder, " ", &str_pointer);

  INCREMENT_IF_STARTS_WITH(str_pointer, ' ')


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

  if (sub_token[0] == 'b'){
    tokenise_b(token, str_pointer, sub_token);
    return ; 
  }

  token->format = INV_F;
  
}


/*
int main(void){
  //= (operand2_t *) malloc(sizeof(operand2_t));
  token_t t ; 
  tokenise_line(&t, "sub r1,r1,#1");
  //printf("1\n");
  //printf("4 %x\n", op.values_oper_t.sh_reg.shifted_vals_t.immediate);

  printf("opcode %s\n", t.contents_f.dp_comp_f.opcode);
  printf("rd %x\n", t.contents_f.dp_comp_f.rd);
  printf("rn %x\n", t.contents_f.dp_comp_f.rn);
  printf("operand2-i %x\n", t.contents_f.dp_comp_f.operand2.i);
  printf("operand2-immediate%x\n", t.contents_f.dp_comp_f.operand2.values_oper_t.immediate);




return 0;
}*/
