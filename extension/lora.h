#ifndef LORA_H
#define LORA_H

#include <stdbool.h>

/*
Usage:
int fd = init_lora();

send_message(fd, message);
bool received  = receive_message(fd, *message);

close_lora(fd);
*/

int init_lora();

void close_lora(int fd);

void send_message(int fd, message_t message);

bool receive_message(int fd, message_t *message);

#endif
