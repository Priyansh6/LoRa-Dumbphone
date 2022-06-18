#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
  int fd;
  if ((fd = serialOpen("dev/ttyS0", 9600)) < 0) {
    perror("Unable to open serial!");
    exit(EXIT_FAILURE);
  }

  if (wiringPiSetup() == -1) {
    perror("Unable to setup wiringPi!");
    exit(EXIT_FAILURE);
  }

  char str[4];
  str[3] = '\0';
  for (int i = 0; i < 180; i++) {
    str = itoa(i, str);
    serialPrintf("%s", str);
    delay(1000);
  }

  printf("Finished sending data\n");

  return 0;
}
