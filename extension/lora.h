#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

int init_lora();

void close_lora(int fd);

void send_message(int fd, message_t message);

bool receive_message(int fd, message_t *message);

#endif
