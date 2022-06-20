#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

#include "utilities.h"

/*
Usage:
int fd = init_lora();

send_message(fd, message);
bool received  = receive_message(fd, &message);

close_lora(fd);
*/

typedef struct pq_node pq_node_t;

struct pq_node {
  message_t message;
  pq_node_t *next;
};

typedef struct pq {
  pq_node_t *head;
} pq_t;

void add_to_pq(pq_t *pq, message_t message);

message_t pop_from_pq(pq_t *pq);

void free_pq(pq_t *pq);

int init_lora();

void close_lora(int fd);

void send_message(int fd, message_t message);

void update_message_queue(int fd);

#endif
