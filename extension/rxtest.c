#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
  int fd;
  int e;
  if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) {
    perror("Unable to open serial!");
    exit(EXIT_FAILURE);
  }

  if (wiringPiSetup() == -1) {
    perror("Unable to setup wiringPi!");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < 240; i++) {
    while ((e = serialDataAvail(fd)) > 0) {
      printf("%c", serialGetchar(fd));
    }
    printf("\nerrno: %s\n", strerror(errno));
    //serialFlush(fd);
    fflush(stdout);
    delay(500);
  }

  printf("Finished receiving data\n");
  serialClose(fd);

  return 0;
}
