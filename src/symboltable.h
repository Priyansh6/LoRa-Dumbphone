#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/*
Usage:
st.get(st, "label1");


*/

typedef struct symbol_table symbol_table_t;

struct symbol_table {
  address *values;
  address (*get)(symbol_table_t st, const char *label);
};


#endif
