#include <stdbool.h>
#include <stdio.h>
#include <wiringPi.h>

#include "keypad.h"

static const int row_pins[NUM_ROWS] = {21, 22, 23, 24};
static const int col_pins[NUM_COLS] = {26, 27, 28};

static const char keys[NUM_ROWS][NUM_COLS][NUM_MODES] = {
  { {'1', '\0', '\0', '\0', '\0'}, {'2', 'A', 'B', 'C', '\0'}, {'3', 'D', 'E', 'F', '\0'} }, 
  { {'4', 'G', 'H', 'I', '\0'}, {'5', 'J', 'K', 'L', '\0'}, {'6', 'M', 'N', 'O', '\0'} }, 
  { {'7', 'P', 'Q', 'R', 'S'}, {'8', 'T', 'U', 'V', '\0'}, {'9', 'W', 'X', 'Y', 'Z'} }, 
  { {'*', '\0', '\0', '\0', '\0'}, {'0', '\0', '\0', '\0', '\0'}, {'#', '\0', '\0', '\0', '\0'} }
};

// Returns number of modes a particular key has at row and col
static int num_modes(int row, int col) {
  char cell = keys[row][col][0];
  switch (cell) {
    case '1':
    case '*':
    case '0':
    case '#':
      return 1;
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '8':
      return 4;
    default:
      return NUM_MODES;
  }
}

void init_keypad(void) {
  // Sets column pins to GPIO output pins
  for (int c = 0; c < NUM_COLS; c++) {
    pinMode(col_pins[c], OUTPUT);   
    digitalWrite(col_pins[c], HIGH);
  }

  // Sets row pins to GPIO input pins with pull-up resistors
  for (int r = 0; r < NUM_ROWS; r++) {
    pinMode(row_pins[0], INPUT);   
    pullUpDnControl(row_pins[r], PUD_UP);
  }
}

key_t mode_read_key(int mode) {
  key_t key_read = { .key_value = '\0', .row = -1, .col = -1 };

  // Loop through each column setting column output to low and then checking
  // row inputs for button press
  for (int c = 0; c < NUM_COLS; c++) {
    digitalWrite(col_pins[c], LOW);

    for (int r = 0; r < NUM_ROWS; r++) {
      if (digitalRead(row_pins[r]) == LOW) {
        key_read.key_value = keys[r][c][mode];
        key_read.row = r;
        key_read.col = c;
        break;
      }
    }

    digitalWrite(col_pins[c], HIGH);

    if (key_read.key_value) {
       break;
    }
  }
  return key_read;
}

char read_key(void) {
  key_t last_key = { .key_value = '\0', .row = -1, .col = -1 };
  key_t key;
  int mode = 0;
  int time_elapsed = 0;
  bool first = true;

  while (time_elapsed < MODE_CHANGE_WAIT_TIME) {
    key = mode_read_key(mode);
    if (key.key_value) {
      if (first) {
        last_key = key;
        first = false;
      }
      if (key.row == last_key.row && key.col == last_key.col) {
        mode = (mode + 1) % num_modes(key.row, key.col);
        last_key = key;
        time_elapsed = 0;
        printf("current value: %c\n", key.key_value);
        printf("mode: %d\n", mode);
      } else {
        return last_key.key_value;
      }
    }
    delay(READ_DELAY);
    time_elapsed += READ_DELAY;
  }

  return last_key.key_value;
}

int main(void) {
  wiringPiSetup();
  init_keypad();

  while(true) {
    char x = read_key();

    if (x) {
      printf("pressed: %c\n", x);
    } else {
      printf("no key pressed\n");
    }
  }

  return 0;
}