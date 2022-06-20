#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

#include "utilities.h"

/*
Usage:
int fd = init_lora();
pq_t *pq = alloc_pq();

message_t temp;

poll_messages(fd, pq, );
while (!is_empty_pq(pq)) {
  message_t m = pop_from_pq(pq);
  // output m
}

send_message(fd, pq, another_message);

close_lora(fd);
free_pq(pq);
*/

typedef struct pq_node pq_node_t;

struct pq_node {
  message_t message;
  pq_node_t *next;
};

typedef struct pq {
  pq_node_t *head;
} pq_t;

pq_t *alloc_pq(); 

void free_pq(pq_t *pq);

bool is_empty_pq(pq_t *pq);

void add_to_pq(pq_t *pq, message_t message);

message_t pop_from_pq(pq_t *pq);

int init_lora();

void close_lora(int fd);

void send_message(int fd, pq_t *pq, message_t message);

void poll_messages(int fd, pq_t *pq, message_t *temp);

void init_message(message_t *m);

#endif
