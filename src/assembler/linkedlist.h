#ifndef LINKEDLIST_H
#define LINKEDLIST_H

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

linked_list_t *alloc_linked_list();
void free_linked_list(linked_list_t *ll);
address lookup_label(linked_list_t *ll, const char *label);
void add_to_linked_list(linked_list_t *ll, const char *label, address addr);
void pprint_linked_list(linked_list_t *ll);

#endif
