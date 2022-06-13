#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"



// START  MACROS ------------------------------------------------------------
#define INCREMENT_IF_STARTS_WITH(chr, val) while (true) { if (chr[0] == val) {chr++;} else {break;} } 
#define SKIP_CHARS(chr, val) chr += val;

#define SET_IN_TOKEN(space_name, struct_name, convertion_function)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = convertion_function(sub_token); \
}

#define SET_IN_TOKEN_STR(space_name, struct_name)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  strcpy(token->contents_f.struct_name.space_name, sub_token);\
}

#define  GOTO_TYPE_FUNC(str_case ,dest)\
if (strcmp(str_case, sub_token) == 0) { \
  dest(token, sub_token);\
  return;\
}

// END MACROS ------------------------------------------------------------

word toimmediate(char *str) {
  INCREMENT_IF_STARTS_WITH(str, ' ')
  str++;
  return strlen(str) >= 2 && str[1] == 'x' ? strtoul(str + 2, NULL, 16) : strtoul(str, NULL, 10);
}

byte get_reg_number(char *reg){
  INCREMENT_IF_STARTS_WITH(reg, ' ')
  INCREMENT_IF_STARTS_WITH(reg, 'r')
  return strtoul(reg, NULL, 10);
}



void get_shift(shift_t *shift, char *str_holder){
  size_t line_length = strlen(str_holder);

  char opr_arr[line_length + 1 ];
  char *opr = &opr_arr[0];

  if (line_length  == 0 || line_length == 1 ) {
    shift->i = false;
    shift->values_oper_t.sh_reg.is_reg = false; 
    shift->values_oper_t.sh_reg.shifted_vals_t.immediate = 0;
    return;
  }

  strcpy(opr, str_holder);

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


}


void get_addr(address_s_t *addr, char *str_holder){  
  size_t line_length = strlen(str_holder);

  char add_addr[line_length + 1];
  char *add = &add_addr[0];

  strcpy(add, str_holder);
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

        if (rest[0] == '-'){
          addr->sighn = false;
        } else if (rest[1] == '-') {
          addr->sighn = false;
          SKIP_CHARS(rest, 1);
        } else {
          addr->sighn = true;
        }

        INCREMENT_IF_STARTS_WITH(rest, '+')
        INCREMENT_IF_STARTS_WITH(rest, '-')

        get_shift(&addr->values_addr_t.shift, rest);
      }
    } else {
      addr->p = true; 

      INCREMENT_IF_STARTS_WITH(rest, ',')
      INCREMENT_IF_STARTS_WITH(rest, ' ')
      
      if (rest[0] == '-'){
          addr->sighn = false;
      } else if (rest[1] == '-') {
          addr->sighn = false;
          rest[1] = '#';
          SKIP_CHARS(rest, 1)
      } else {
          addr->sighn = true;
      }
      
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


void tokenise_comp(token_t *token, char *sub_token){
  token->format = DP_COMP_F;

  INCREMENT_IF_STARTS_WITH(sub_token, ' ')
  SET_IN_TOKEN_STR(opcode, dp_comp_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rd, dp_comp_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rn, dp_comp_f, get_reg_number)

  sub_token = strtok(NULL, "\0"); 
  if(strlen(sub_token) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_comp_f.operand2 , sub_token);
  }

}


void tokenise_mov(token_t *token, char *sub_token){
  token->format = DP_MOV_F;

  sub_token = strtok(NULL, ",");
  SET_IN_TOKEN(rd, dp_mov_f, get_reg_number)

  sub_token = strtok(NULL, "\0");
  if(strlen(sub_token) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_mov_f.operand2 , sub_token);
  }
}

void tokenise_ncomp(token_t *token, char *sub_token){
  token->format = DP_NCOMP_F;

  INCREMENT_IF_STARTS_WITH(sub_token, ' ')
  SET_IN_TOKEN_STR(opcode, dp_ncomp_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rn, dp_ncomp_f, get_reg_number)

  sub_token = strtok(NULL, "\0"); 
  if(strlen(sub_token) == 0){
    token->format = INV_F;
    return;
  } else {
    get_shift(&token->contents_f.dp_ncomp_f.operand2 , sub_token);
  }
}

void tokenise_m(token_t *token, char *sub_token){
  token->format = M_F;

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rd, m_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rm, m_f, get_reg_number)
  
  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rs, m_f, get_reg_number)
}

void tokenise_ma(token_t *token, char *sub_token){
  token->format = MA_F;

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rd, ma_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rm, ma_f, get_reg_number)
  
  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rs, ma_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rn, ma_f, get_reg_number)
}


void tokenise_sdt(token_t *token, char *sub_token){
  token->format = SDT_F;

  INCREMENT_IF_STARTS_WITH(sub_token, ' ')
  SET_IN_TOKEN_STR(expr, sdt_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(rd, sdt_f, get_reg_number)

  sub_token = strtok(NULL, "\0"); 
  if(strlen(sub_token) == 0){
    token->format = INV_F;
    return;
  } else {
    get_addr(&token->contents_f.sdt_f.addr , sub_token);
  }
}


void tokenise_b(token_t *token, char *sub_token){
  token->format = B_F;

  SET_IN_TOKEN_STR(cond, b_f)
  sub_token = strtok(NULL, " "); 
  SET_IN_TOKEN_STR(offset, b_f)
}

void tokenise_andeq(token_t *token, char *sub_token){
  token->format = ANDEQ_F;
}

void tokenise_lsl(token_t *token, char *sub_token){
  token->format = LSL_F;

  sub_token = strtok(NULL, ",");
  SET_IN_TOKEN(rn, lsl_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(expr, lsl_f, toimmediate)
}


void tokenise_line(token_t *token, const char *line){

  size_t line_length = strlen(line);
 
  char *sub_token;
  char str_holder[line_length + 1];
  strcpy(str_holder, line);

  printf("%s\n", str_holder);

  sub_token = strtok(str_holder, " ");

  printf("%s\n", line);

  printf("1\n");
  
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
    tokenise_b(token, sub_token);
    return ; 
  }

  token->format = INV_F;
  
}

