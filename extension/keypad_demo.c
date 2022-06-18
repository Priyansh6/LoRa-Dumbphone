#include <stdio.h>
#include <wiringPi.h>

#include "keypad.h"

int main(void) {
  wiringPiSetup();
  init_keypad();

  while(1) {
    char x = read_key();
    if (x) {
      printf("Pressed: %c\n", x);
    } else {
      printf("No key pressed\n");
    }
  }
  return 0;
}
