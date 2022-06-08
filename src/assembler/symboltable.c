#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboltable.h"

int hash_label(const char *label) {
  int s = 0;
  for (int i = 0; i < strlen(label); i++) {
    s += label[i];
  }
  return s % NUM_BUCKETS;
}

linked_list_t *get_bucket(symbol_table_t *st, const char *label) {
  int hash = hash_label(label);
  return st->buckets[hash];
}

address get_label(symbol_table_t *st, const char *label) {
  return lookup_label(get_bucket(st, label), label);
}

void add_label(symbol_table_t *st, const char *label, address addr) {
  linked_list_t *bucket = get_bucket(st, label);
  add_to_linked_list(bucket, label, addr);
}

void init_symbol_table(symbol_table_t *st, address (*get)(symbol_table_t *st, const char *label), void (*add)(symbol_table_t *st, const char *label, address addr)) {
  st->buckets = (linked_list_t **) calloc(NUM_BUCKETS, sizeof(linked_list_t *));

  if (st->buckets == NULL) {
    perror("Unable to allocate symbol table buckets");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < NUM_BUCKETS; i++) {
    st->buckets[i] = alloc_linked_list();
  }

  st->get = get;
  st->add = add;
}

void free_symbol_table(symbol_table_t *st) {
  for (int i = 0; i < NUM_BUCKETS; i++) {
    free_linked_list(st->buckets[i]); 
  }
  free(st->buckets);
}

void pprint_symbol_table(symbol_table_t st) {
  for (int i = 0; i < NUM_BUCKETS; i++) {
    printf("Bucket %d:\n", i);
    pprint_linked_list(st.buckets[i]);  
  }
}
