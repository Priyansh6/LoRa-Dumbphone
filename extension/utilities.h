#ifndef UTILITIES_H
#define UTILITIES_H

#include <time.h>

#define MESSAGE_LENGTH 250
#define TIMESTAMP_LENGTH 4
#define SENDER_LENGTH 20

typedef struct message {
  char contents[MESSAGE_LENGTH];
  char sender[SENDER_LENGTH];
  time_t t;
} message_t;

// sender@2398732987contents\0



// sender@329874293874contents\0
// helloworld\sender\987329874239874\0

#endif
