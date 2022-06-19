#ifndef UTILITIES_H
#define UTILITIES_H

#include <time.h>

#define MESSAGE_LENGTH 250
#define SENDER_LENGTH 20

typedef struct tm tm_t;

typedef struct message {
  char* contents;
  char sender[SENDER_LENGTH];
  tm_t time;
  int msg_len;
} message_t;

#endif