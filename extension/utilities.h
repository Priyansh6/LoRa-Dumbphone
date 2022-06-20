#ifndef UTILITIES_H
#define UTILITIES_H

#define MESSAGE_LENGTH 250
#define SENDER_LENGTH 20

typedef struct message {
  char contents[MESSAGE_LENGTH];
  char sender[SENDER_LENGTH];
  unsigned int time;
} message_t;

#endif
