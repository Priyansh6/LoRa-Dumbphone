#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "lora.h"

#define SERIAL_PATH "/dev/ttyS0"
#define BAUD_RATE 9600

enum ReadStage {CONTENTS, SENDER, TIMESTAMP};

pq_t *alloc_pq() {
  pq_t *pq = (pq_t *) malloc(sizeof(pq_t));
  pq->head = NULL;
  return pq;
}

pq_node_t *alloc_pq_node(message_t message) {
  pq_node_t *pqn = (pq_node_t *) malloc(sizeof(pq_node_t));
  pqn->message = message;
  pqn->next = NULL;
  return pqn;
}

void free_pq_node(pq_node_t *pqn) {
  if (pqn->next == NULL) {
    return;
  }
  free_pq_node(pqn->next);
}

void free_pq(pq_t *pq) {
  free_pq_node(pq->head);
}

bool is_empty_pq(pq_t *pq) {
  return pq->head == NULL;
}

void print_pq_node(pq_node_t *pqn) {
  printf("Sender: %s\n", pqn->message.sender);
  printf("Contents: %s\n", pqn->message.contents);
  printf("Time: %d\n", pqn->message.time);
  
  if (pqn->next != NULL) {
    print_pq_node(pqn->next);
  }
  printf("\n\n");
}

void print_pq(pq_t *pq) {
  if (pq->head != NULL) {
    print_pq_node(pq->head);
  }
}

void add_to_pq(pq_t *pq, message_t message) {
  pq_node_t *node = alloc_pq_node(message);
  pq_node_t *curr = pq->head;

  if (curr == NULL || node->message.time < curr->message.time) {
    pq->head = node;
    node->next = curr;
    return;
  }

  pq_node_t *prev = NULL;

  while (curr != NULL && node->message.time > curr->message.time) {
    prev = curr;
    curr = curr->next;
  }

  node->next = curr;
  prev->next = node;
}

message_t pop_from_pq(pq_t *pq) {
  pq_node_t *pqn = pq->head;
  message_t message = pqn->message;
  pq->head = pqn->next;
  free(pqn);
  return message;
}

int init_lora() {
  int fd;
  if ((fd = serialOpen(SERIAL_PATH, BAUD_RATE)) < 0) {
    perror("Unable to open serial!");
    exit(EXIT_FAILURE);
  }
  return fd;
}

void close_lora(int fd) {
  serialClose(fd); 
}

void init_message(message_t *m) {
  memset(m->contents, '\0', MESSAGE_LENGTH); 
  memset(m->sender, '\0', SENDER_LENGTH); 
  m->t = 0;
}

void send_message(int fd, pq_t *pq, message_t message) {
  for (int i = 0; i < strlen(message->sender); i++) {
    serialPutchar(fd, message->sender[i]);
  }

  serialPutchar(fd, '@');

  for (int i = 0; i < TIMESTAMP_LENGTH; i++) {
    serialPutchar(fd, message->t & (0xFF << i));
  }

  for (int i = 0; i < strlen(message->contents); i++) {
    serialPutchar(fd, message->contents[i]);
  }

  serialPutchar(fd, '\0');

  add_to_pq(pq, message);
}

void poll_messages(int fd, pq_t *pq, message_t *temp) {
  static enum ReadStage stage = SENDER;
  static int bytes_read = 0;
  char c;

  while (serialDataAvail(fd) > 0) {
    c = serialGetchar(fd);

    switch (stage) {
      case CONTENTS:
        if (c == '\0') {
          message_t final_message = {temp->contents, temp->sender, temp->t};
          send_message(fd, pq, final_message); 
          init_message(temp);
        } else {
          temp->contents[strlen(temp->contents)] = c;
        }
        break;
      case SENDER:
        if (c == '@') {
          stage = TIMESTAMP;
        } else {
          temp->sender[strlen(temp->sender)] = c;
        }
        break;
      case TIMESTAMP:
        temp->t |= c << (2 * (TIMESTAMP_LENGTH - 1 - bytes_read));
        bytes_read++;
        if (bytes_read == 4) {
          stage = CONTENTS; 
        }
        break;
    }
  }
}

bool receive_message(int fd, message_t *message) {
  bool received = false;
  int i = 0;
  while (serialDataAvail(fd) > 0) {
    received = true;
    message->contents[i] = serialGetchar(fd); 
    i++;
  }
  message->contents[i] = '\0';
  return received;
}

/*
int main() {
  message_t m1 = {"m1", "sender1", 2};
  message_t m2 = {"m2", "sender2", 7};
  message_t m3 = {"m3", "sender3", 4};
  message_t m4 = {"m4", "sender1", 1};

  // ordering is t4, t1, t3, t2

  pq_t *pq = alloc_pq();
  add_to_pq(pq, m1);
  print_pq(pq);
  add_to_pq(pq, m2);
  print_pq(pq);
  add_to_pq(pq, m3);
  print_pq(pq);
  add_to_pq(pq, m4);
  print_pq(pq);

  message_t q1 = pop_from_pq(pq);
  printf("%s\n", q1.contents);
  free_pq(pq);
}
*/
