#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokeniser.h"

// START  MACROS ------------------------------------------------------------
#define INCREMENT_IF_STARTS_WITH(chr, val) while (true) { if (chr[0] == val) {chr++;} else {break;} } 
#define SKIP_CHARS(chr, val) chr += val;

#define SET_IN_TOKEN(token, sub_token, space_name, struct_name, convertion_function)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  token->contents_f.struct_name.space_name = convertion_function(sub_token); \
}

#define SET_IN_TOKEN_STR(token, sub_token, space_name, struct_name)\
if(sub_token == NULL){\
  token->format = INV_F;\
  return;\
} else {\
  strcpy(token->contents_f.struct_name.space_name, sub_token);\
}

#define GOTO_TYPE_FUNC(token, sub_token, str_case, dest)\
if (strcmp(str_case, sub_token) == 0) { \
  dest(token, sub_token);\
  return;\
}

// END MACROS ------------------------------------------------------------

word to_immediate(char *str) {
  INCREMENT_IF_STARTS_WITH(str, ' ')
  SKIP_CHARS(str, 1)
  return strlen(str) >= 2 && str[1] == 'x' ? strtoul(str + 2, NULL, 16) : strtoul(str, NULL, 10);
}

byte get_reg_number(char *reg) {
  INCREMENT_IF_STARTS_WITH(reg, ' ')
  INCREMENT_IF_STARTS_WITH(reg, 'r')
  return strtoul(reg, NULL, 10);
}

void get_shift(shift_t *shift, char *str_holder){
  size_t line_length = strlen(str_holder);

  char opr_arr[line_length + 1];
  char *opr = &opr_arr[0];

  if (line_length == 0 || line_length == 1) {
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
      } else if (!strncmp(rest, "lsr", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 1;
      } else if (!strncmp(rest, "asr", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 2;
      } else if (!strncmp(rest, "ror", 3)) {
        shift->values_oper_t.sh_reg.shift_type = 3;
      }

      SKIP_CHARS(rest, 4)

      if (rest[0] == 'r') {
        SKIP_CHARS(rest, 1)
        shift->values_oper_t.sh_reg.is_reg = true;
        shift->values_oper_t.sh_reg.shifted_vals_t.rs = strtoul(rest, NULL, 10);
      } else {
        shift->values_oper_t.sh_reg.is_reg = false;
        shift->values_oper_t.sh_reg.shifted_vals_t.immediate = to_immediate(rest);
      }
    } else {
      shift->values_oper_t.sh_reg.shift_type = 0;
      shift->values_oper_t.sh_reg.is_reg = false;
      shift->values_oper_t.sh_reg.shifted_vals_t.immediate = 0;
    }
  } else {
    shift->i = true;
    shift->values_oper_t.immediate = to_immediate(opr);
  }
}

void get_addr(address_s_t *addr, char *str_holder){  
  size_t line_length = strlen(str_holder);

  char addr_str_arr[line_length + 1];
  char *addr_str = &addr_str_arr[0];
  strcpy(addr_str, str_holder);

  INCREMENT_IF_STARTS_WITH(addr_str, ' ')

  addr->sighn = true;

  if (addr_str[0] == '=') {
    addr->i = true;
    addr->values_addr_t.immediate = to_immediate(addr_str);
  } else {
    char *rest;

    addr->i = false;

    INCREMENT_IF_STARTS_WITH(addr_str, '[')
    INCREMENT_IF_STARTS_WITH(addr_str, 'r')
    addr->rn = strtoul(addr_str, &rest, 10);
    
    if (rest[0] == ']') {
      addr->p = false; 

      if (strlen(rest) == 1 ){
        addr->p = true; 
        addr->values_addr_t.shift.i = true; 
        addr->values_addr_t.shift.values_oper_t.immediate = 0; 
        return;
      }
    } else {
      addr->p = true; 

    }
    INCREMENT_IF_STARTS_WITH(rest, ']')
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

void tokenise_comp(token_t *token, char *sub_token){
  token->format = DP_COMP_F;

  INCREMENT_IF_STARTS_WITH(sub_token, ' ')
  SET_IN_TOKEN_STR(token, sub_token, opcode, dp_comp_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rd, dp_comp_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rn, dp_comp_f, get_reg_number)

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
  SET_IN_TOKEN(token, sub_token, rd, dp_mov_f, get_reg_number)

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
  SET_IN_TOKEN_STR(token, sub_token, opcode, dp_ncomp_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rn, dp_ncomp_f, get_reg_number)

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
  SET_IN_TOKEN(token, sub_token, rd, m_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rm, m_f, get_reg_number)
  
  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rs, m_f, get_reg_number)
}

void tokenise_ma(token_t *token, char *sub_token){
  token->format = MA_F;

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rd, ma_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rm, ma_f, get_reg_number)
  
  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rs, ma_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rn, ma_f, get_reg_number)
}

void tokenise_sdt(token_t *token, char *sub_token){
  token->format = SDT_F;

  INCREMENT_IF_STARTS_WITH(sub_token, ' ')
  SET_IN_TOKEN_STR(token, sub_token, expr, sdt_f)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, rd, sdt_f, get_reg_number)

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

  SET_IN_TOKEN_STR(token, sub_token, cond, b_f)
  sub_token = strtok(NULL, " "); 
  SET_IN_TOKEN_STR(token, sub_token, offset, b_f)
}

void tokenise_andeq(token_t *token, char *sub_token){
  token->format = ANDEQ_F;
}

void tokenise_lsl(token_t *token, char *sub_token){
  token->format = LSL_F;

  sub_token = strtok(NULL, ",");
  SET_IN_TOKEN(token, sub_token, rn, lsl_f, get_reg_number)

  sub_token = strtok(NULL, ","); 
  SET_IN_TOKEN(token, sub_token, expr, lsl_f, to_immediate)
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
  
  GOTO_TYPE_FUNC(token, sub_token, "and", tokenise_comp )
  GOTO_TYPE_FUNC(token, sub_token, "eor", tokenise_comp )
  GOTO_TYPE_FUNC(token, sub_token, "sub", tokenise_comp )
  GOTO_TYPE_FUNC(token, sub_token, "rsb", tokenise_comp )
  GOTO_TYPE_FUNC(token, sub_token, "add", tokenise_comp )
  GOTO_TYPE_FUNC(token, sub_token, "orr", tokenise_comp )

  GOTO_TYPE_FUNC(token, sub_token, "mov", tokenise_mov )

  GOTO_TYPE_FUNC(token, sub_token, "tst", tokenise_ncomp )
  GOTO_TYPE_FUNC(token, sub_token, "teq", tokenise_ncomp )
  GOTO_TYPE_FUNC(token, sub_token, "cmp", tokenise_ncomp )

  GOTO_TYPE_FUNC(token, sub_token, "mul", tokenise_m)
  GOTO_TYPE_FUNC(token, sub_token, "mla", tokenise_ma )

  GOTO_TYPE_FUNC(token, sub_token, "ldr", tokenise_sdt )
  GOTO_TYPE_FUNC(token, sub_token, "str", tokenise_sdt )

  GOTO_TYPE_FUNC(token, sub_token, "andeq", tokenise_andeq )
  GOTO_TYPE_FUNC(token, sub_token, "lsl", tokenise_lsl )

  if (sub_token[0] == 'b'){
    tokenise_b(token, sub_token);
    return ; 
  }

  token->format = INV_F;
  
}

