#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "lora.h"

#define SERIAL_PATH "/dev/ttyS0"
#define BAUD_RATE 9600

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

void send_message(int fd, message_t message) {
  serialPrintf(fd, "%s", message.contents);
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

