#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboltable.h"

key_value_list_t *alloc_key_value_list() {
  key_value_list_t *p = (key_value_list_t *) malloc(sizeof(key_value_list_t));
  p->head = NULL;
  return p;
}

void pprint_key_value_node(key_value_node_t *n) {
  printf("Label: %s; Address: %x;\n", n->label, n->addr);
}

key_value_node_t *alloc_key_value_node(const char *label, address addr) {
  key_value_node_t *p = (key_value_node_t *) malloc(sizeof(key_value_node_t));
  if (p == NULL) {
    perror("Unable to alloc key value node");
    exit(EXIT_FAILURE);
  }
  p->label = (char *) malloc(strlen(label) + 1);
  strcpy(p->label, label);
  p->addr = addr;
  p->next = NULL;
  return p;
}

void free_key_value_node(key_value_node_t *n) {
  assert(n != NULL);
  if (n->next != NULL) {
    free_key_value_node(n->next);
  }
  free(n->label);
  free(n);
}

void free_key_value_list(key_value_list_t *ll) {
  if (ll->head != NULL) {
    free_key_value_node(ll->head);
  }
  free(ll);
}
  
void add_to_key_value_list(key_value_list_t *ll, const char *label, address addr) {
  key_value_node_t *curr_head = ll->head; 
  key_value_node_t *new_head = alloc_key_value_node(label, addr); 
  new_head->next = curr_head; 
  ll->head = new_head;
}

address lookup_label(key_value_list_t *ll, const char *label) {
  key_value_node_t *curr = ll->head; 
  assert(curr != NULL);
  while (strcmp(label, curr->label) != 0) {
    curr = curr->next;
  }
  assert(curr != NULL);
  return curr->addr;
}

void pprint_key_value_list(key_value_list_t *ll) {
  key_value_node_t *curr = ll->head;
  while (curr != NULL) {
    pprint_key_value_node(curr);
    curr = curr->next;
  }
}

int hash_label(const char *label) {
  int s = 0;
  for (int i = 0; i < strlen(label); i++) {
    s += label[i];
  }
  return s % NUM_BUCKETS;
}

key_value_list_t *get_bucket(symbol_table_t *st, const char *label) {
  int hash = hash_label(label);
  return st->buckets[hash];
}

address get_label(symbol_table_t *st, const char *label) {
  return lookup_label(get_bucket(st, label), label);
}

void add_label(symbol_table_t *st, const char *label, address addr) {
  key_value_list_t *bucket = get_bucket(st, label);
  add_to_key_value_list(bucket, label, addr);
}

void init_symbol_table(symbol_table_t *st, 
                       address (*get)(symbol_table_t *st, const char *label), 
                       void (*add)(symbol_table_t *st, const char *label, 
                       address addr)) {
  st->buckets = (key_value_list_t **) calloc(NUM_BUCKETS, sizeof(key_value_list_t *));

  if (st->buckets == NULL) {
    perror("Unable to allocate symbol table buckets");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < NUM_BUCKETS; i++) {
    st->buckets[i] = alloc_key_value_list();
  }

  st->get = get;
  st->add = add;
}

void free_symbol_table(symbol_table_t *st) {
  for (int i = 0; i < NUM_BUCKETS; i++) {
    free_key_value_list(st->buckets[i]); 
  }
  free(st->buckets);
}

void pprint_symbol_table(symbol_table_t st) {
  for (int i = 0; i < NUM_BUCKETS; i++) {
    printf("Bucket %d:\n", i);
    pprint_key_value_list(st.buckets[i]);  
  }
}