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

  for (int i = 0; i < 180; i++) {
    while (serialDataAvail(fd)) {
      printf("%c", serialGetchar(fd));
    }
    delay(1000);
  }

  printf("Finished receiving data\n");

  return 0;
}
