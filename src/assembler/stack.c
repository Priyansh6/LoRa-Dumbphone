#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

stack_t *alloc_stack() {
  stack_t *p = (stack_t *) malloc(sizeof(stack_t));
  p->head = NULL;
  return p;
}

void pprint_stack_node(stack_node_t *n) {
  printf("Word: %x;\n", n->w);
}

stack_node_t *alloc_stack_node(word w) {
  stack_node_t *p = (stack_node_t *) malloc(sizeof(stack_node_t));
  if (p == NULL) {
    perror("Unable to alloc stack node");
    exit(EXIT_FAILURE);
  }
  p->w = w;
  p->next = NULL;
  return p;
}

void free_stack_node(stack_node_t *n) {
  assert(n != NULL);
  if (n->next != NULL) {
    free_stack_node(n->next);
  }
  free(n);
}

void free_stack(stack_t *s) {
  if (s->head != NULL) {
    free_stack_node(s->head);
  }
  free(s);
}

void push(stack_t *s, word w) {
  stack_node_t *curr_head = s->head;
  stack_node_t *new_head = alloc_stack_node(w);
  new_head->next = curr_head;
  s->head = new_head;
}

word pop(stack_t *s) {
  word w = s->head->w;
  stack_node_t *old_head = s->head;
  s->head = s->head->next;
  free(old_head);
  return w;
}

size_t size(stack_t *s) {
  size_t n = 0;
  stack_node_t *curr = s->head;
  while (curr != NULL) {
    n++;
    curr = curr->next;
  }
  return n;
}

void pprint_stack(stack_t *s) {
  stack_node_t *curr = s->head;
  while (curr != NULL) {
    pprint_stack_node(curr);
    curr = curr->next;
  }
}
