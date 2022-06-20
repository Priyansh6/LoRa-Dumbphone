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
  if (pq->head != NULL) {
    free_pq_node(pq->head);
  }
}

bool is_empty_pq(pq_t *pq) {
  return pq->head == NULL;
}

void print_pq_node(pq_node_t *pqn) {
  printf("Sender: %s\n", pqn->message.sender);
  printf("Contents: %s\n", pqn->message.contents);
  //printf("Time: %x\n", pqn->message.t);
  
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

  if (curr == NULL || node->message.t < curr->message.t) {
    pq->head = node;
    node->next = curr;
    return;
  }

  pq_node_t *prev = NULL;

  while (curr != NULL && node->message.t >= curr->message.t) {
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
  // Sending the sender info
  for (int i = 0; i < strlen(message.sender); i++) {
    serialPutchar(fd, message.sender[i]);
  }

  serialPutchar(fd, '@');

  // Send each byte
  // 0xA037B22B
  for (int i = 0; i < TIMESTAMP_LENGTH; i++) {
    //printf("%x\n", (message.t >> (2 * i)) & 0xFF);
    serialPutchar(fd, (message.t >> (8 * i)) & 0xFF);
  }

  for (int i = 0; i < strlen(message.contents); i++) {
    serialPutchar(fd, message.contents[i]);
  }

  serialPutchar(fd, '\0');

  add_to_pq(pq, message);
  //printf("Sent message\n");
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
          message_t final_message;
          memcpy(&final_message, temp, sizeof(message_t));
	        add_to_pq(pq, final_message);
          init_message(temp);
          bytes_read = 0;
          stage = SENDER;
        } else {
          temp->contents[strlen(temp->contents)] = c;
          //temp->contents[strlen(temp->contents) +1] = '\0';
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
        temp->t |= c << (8 * (TIMESTAMP_LENGTH - 1 - bytes_read));
        //printf("%x %x\n", c, temp->t);
        bytes_read++;
        if (bytes_read >= 4) {
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
  int fd = init_lora();
  pq_t *pq = alloc_pq();
  message_t temp;
  init_message(&temp);
  delay(10000);
  printf("going\n");
  
  message_t m1 = {"contents contents", "john", 10};
  send_message(fd, pq, m1);
  print_pq(pq);

  delay(2500);

  message_t m2 = {"a slightly longer message for testing", "sean", 25};
  send_message(fd, pq, m2);
  print_pq(pq);

  delay(2500);

  message_t m3 = {"tiny msg", "small", 2};
  send_message(fd, pq, m3);
  print_pq(pq);
  

  
  poll_messages(fd, pq, &temp);
  print_pq(pq);

  delay(4000);

  poll_messages(fd, pq, &temp);
  print_pq(pq);
  //

  close_lora(fd);
  free_pq(pq);
}
*/
