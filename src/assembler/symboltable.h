#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define NUM_BUCKETS 100

#include "../utilities.h"

/*
Usage:
// Declare and initialise symbol table
symbol_table_t st;
init_symbol_table(&st, get_label, add_label); 

// Add key-value pair of "label1" and 0x00329 to table
st.add(&st, "label1", 0x0329);

// Get value associated with key "label1" (returns 0x0329)
st.get(&st, "label1");

// Free symbol table after use
free_symbol_table(&st);
*/
struct key_value_node;

typedef struct key_value_list {
  struct key_value_node *head;
} key_value_list_t;

typedef struct key_value_node {
  char *label;
  address addr;
  struct key_value_node *next;  
} key_value_node_t; 

typedef struct symbol_table symbol_table_t;

struct symbol_table {
  key_value_list_t **buckets;
  address (*get)(symbol_table_t *st, const char *label);
  void (*add)(symbol_table_t *st, const char *label, address addr);
};

void init_symbol_table(symbol_table_t *st, 
                       address (*get)(symbol_table_t *st, const char *label), 
                       void (*add)(symbol_table_t *st, const char *label, 
                       address addr));
void free_symbol_table(symbol_table_t *st);

address get_label(symbol_table_t *st, const char *label); 
void add_label(symbol_table_t *st, const char *label, address addr);

void pprint_symbol_table(symbol_table_t st);
#endif
