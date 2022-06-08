#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"

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

void pprint_linked_list(linked_list_t *ll) {
  linked_list_node_t *curr = ll->head;
  while (curr != NULL) {
    pprint_node(curr);
    curr = curr->next;
  }
}
