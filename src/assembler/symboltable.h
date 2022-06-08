#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define NUM_BUCKETS 100

#include "../utilities.h"

struct linked_list_node;

typedef struct linked_list {
  struct linked_list_node *head;
} linked_list_t;

typedef struct linked_list_node {
  char *label;
  address addr;
  struct linked_list_node *next;  
} linked_list_node_t; 

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


#endif
