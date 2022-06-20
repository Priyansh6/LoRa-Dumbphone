#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>

int main() {
  int fd;
  if ((fd = serialOpen("/dev/ttyS0", 9600)) < 0) {
    perror("Unable to open serial!");
    exit(EXIT_FAILURE);
  }

  if (wiringPiSetup() == -1) {
    perror("Unable to setup wiringPi!");
    exit(EXIT_FAILURE);
  }

  serialPrintf(fd, "%s", "hamish@");
  serialPutchar(fd, 0);
  serialPutchar(fd, 0);
  serialPutchar(fd, 3); // 60
  serialPutchar(fd, 0xC);
  serialPrintf(fd, "%s", "a bog standard message");
  serialPutchar(fd, '\0');
  printf("sent msg 1\n");

  delay(2000);


  serialPrintf(fd, "%s", "priyansh@");
  serialPutchar(fd, 0);
  serialPutchar(fd, 1); // 290
  serialPutchar(fd, 2); // 
  serialPutchar(fd, 0x2);
  serialPrintf(fd, "old rosie");
  serialPutchar(fd, '\0');

  printf("sent msg 2\n");

  /*
  char str[5];
  str[3] = '\n';
  str[4] = '\0';
  for (int i = 0; i < 180; i++) {
    sprintf(str, "%d", i);
    printf("%s\n", str);
    serialPrintf(fd, "%s", str);
    delay(1000);
  }
  */

  printf("Finished sending data\n");

  return 0;
}
