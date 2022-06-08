#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symboltable.h"

linked_list_t *alloc_linked_list() {
  linked_list_t *p = (linked_list_t *) malloc(sizeof(linked_list_t));
  p->head = NULL;
}

void pprint_node(linked_list_node_t *n) {
  printf("Label: %s; Address: %x;\n", n->label, n->addr);
}

linked_list_node_t *alloc_linked_list_node(const char *label, address addr) {
  linked_list_node_t *p = (linked_list_node_t *) malloc(sizeof(linked_list_node_t));
  if (p == NULL) {
    perror("Unable to alloc linked list node");
    exit(EXIT_FAILURE);
  }
  p->label = (char *) malloc(strlen(label) + 1);
  strcpy(p->label, label);
  p->addr = addr;
  p->next = NULL;
  return p;
}

void free_linked_list_node(linked_list_node_t *n) {
  assert(n != NULL);
  if (n->next != NULL) {
    free_linked_list_node(n->next);
  }
  free(n->label);
  free(n);
}

void free_linked_list(linked_list_t *ll) {
  if (ll->head != NULL) {
    free_linked_list_node(ll->head);
  }
  free(ll);
}

void add_to_linked_list(linked_list_t *ll, const char *label, address addr) {
  linked_list_node_t *curr_head = ll->head; 
  linked_list_node_t *new_head = alloc_linked_list_node(label, addr); 
  new_head->next = curr_head; 
  ll->head = new_head;
}

address lookup_label(linked_list_t *ll, const char *label) {
  linked_list_node_t *curr = ll->head; 
  assert(curr != NULL);
  while (strcmp(label, curr->label) != 0) {
    curr = curr->next;
  }
  assert(curr != NULL);
  return curr->addr;
}

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
