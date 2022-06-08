#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define NUM_BUCKETS 100

#include "../utilities.h"
#include "linkedlist.h"

/*
Usage:
st.get(&st, "label1");
st.add(&st, "label1", 0x0329);

*/

typedef struct symbol_table symbol_table_t;

struct symbol_table {
  linked_list_t **buckets;
  address (*get)(symbol_table_t *st, const char *label);
  void (*add)(symbol_table_t *st, const char *label, address addr);
};

void init_symbol_table(symbol_table_t *st, address (*get)(symbol_table_t *st, const char *label), void (*add)(symbol_table_t *st, const char *label, address addr));
void free_symbol_table(symbol_table_t *st);

address get_label(symbol_table_t *st, const char *label); 
void add_label(symbol_table_t *st, const char *label, address addr);

void pprint_symbol_table(symbol_table_t st);
#endif
