#ifndef STACK_H
#define STACK_H

#include "../utilities.h"

/*
Usage:
stack_t *s = alloc_stack();
push(s, 0x1);
push(s, 0x2);
word w = pop(s);
size_t s = size(s);
free_stack(s);
*/

struct stack_node;

typedef struct stack {
  struct stack_node *head;
} stack_t;

typedef struct stack_node {
  word w;
  struct stack_node *next;
} stack_node_t;

stack_t *alloc_stack();
void free_stack(stack_t *s);
void push(stack_t *s, word w);
word pop(stack_t *s);
void pprint_stack(stack_t *s);
word size(stack_t *s);

#endif