#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include "keypad.h"

#define MESSAGE_LENGTH 250
#define SENDER_LENGTH 20

typedef struct tm tm_t;

typedef struct message {
  char contents[MESSAGE_LENGTH];
  char sender[SENDER_LENGTH];
  tm_t time;
} message_t;

int main(void) {
  wiringPiSetup();
  init_keypad();

  bool quit = false;

  /* WELCOME MESSAGE */

  /* Help message macro */

  /* 1. Receive from specific sender 
     2. Receive from all
     3. Set sender id
     4. Chat
     */


  while(!quit) {

  }
}