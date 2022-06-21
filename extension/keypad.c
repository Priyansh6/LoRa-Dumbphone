#include <stdio.h>
#include <time.h>
#include <wiringPi.h>

#include "keypad.h"

static const int row_pins[NUM_ROWS] = {21, 22, 23, 24};
static const int col_pins[NUM_COLS] = {26, 27, 28};

static const char keys[NUM_ROWS][NUM_COLS][NUM_MODES] = {
  { {'1', '\0', '\0', '\0', '\0'}, {'2', 'A', 'B', 'C', '\0'}, {'3', 'D', 'E', 'F', '\0'} }, 
  { {'4', 'G', 'H', 'I', '\0'}, {'5', 'J', 'K', 'L', '\0'}, {'6', 'M', 'N', 'O', '\0'} }, 
  { {'7', 'P', 'Q', 'R', 'S'}, {'8', 'T', 'U', 'V', '\0'}, {'9', 'W', 'X', 'Y', 'Z'} }, 
  { {'*', '\0', '\0', '\0', '\0'}, {'0', ' ', '|', '\0', '\0'}, {'#', '\0', '\0', '\0', '\0'} }
};

// Returns number of modes a particular key has at row and col
static int button_num_modes(int row, int col) {
  char cell = keys[row][col][0];
  switch (cell) {
    case '1':
    case '*':
    case '#':
      return 1;
    case '0':
      return 3;
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

button_t mode_read_key(int *mode) {
  button_t key_read = { .value = '\0', .row = -1, .col = -1 };

  // Loop through each column setting column output to low and then checking row inputs for button
  // press
  for (int c = 0; c < NUM_COLS; c++) {
    digitalWrite(col_pins[c], LOW);

    for (int r = 0; r < NUM_ROWS; r++) {
      if (digitalRead(row_pins[r]) == LOW) {
        *mode = *mode >= button_num_modes(r, c) ? 0 : *mode;
        key_read.value = keys[r][c][*mode];
        key_read.row = r;
        key_read.col = c;
        break;
      }
    }

    digitalWrite(col_pins[c], HIGH);

    if (key_read.value) {
       break;
    }
  }
  return key_read;
}

static void reset_read_key_mid(button_t *last_key, 
                              int *mode, 
                              bool *accept_input, 
                              bool *first) {
  *last_key = (button_t) { .value = '\0', .row = -1, .col = -1 };
  *mode = 0;
  *accept_input = true;
  *first = true;
}

bool read_key_mid(char *mid_key, int *start_time) {
  static button_t last_key = { .value = '\0', .row = -1, .col = -1 };
  static int mode = 0;
  static bool accept_input = true;
  static bool first = true;

  // If button is still waiting for input, then read the keypad. If not then return the last key
  // pressed or '\0' if no key was pressed
  if (millis() - *start_time < BUTTON_WAIT_TIME) {
    button_t key = mode_read_key(&mode);

    // If button is pressed change mode or return last button pressed if it is different
    if (key.value && accept_input) {
      
      if (first) {
        last_key = key;
        first = false;
      }

      if (key.row == last_key.row && key.col == last_key.col) { // Pressed same button
        *mid_key = key.value;
        mode = (mode + 1) % button_num_modes(key.row, key.col);
        last_key = key;
        *start_time = millis();
        accept_input = false;
      } else { // Pressed different button
        *mid_key = last_key.value;
        reset_read_key_mid(&last_key, &mode, &accept_input, &first);
        return true;
      }
    } else if (!key.value) { // Didn't press anything
      accept_input = true;
    }
  } else {
    reset_read_key_mid(&last_key, &mode, &accept_input, &first);
    return true;
  }

  // Required to ensure letting go of button doesn't get read as a signal
  delay(1);
  return false;
}

char read_key(void) {
  char last_result = '\0';
  char result = '\0';
  int start_time = millis();
  while (!read_key_mid(&result, &start_time)) {
    if (result && result != last_result) {
      printf("Switched to: %c\n", result);
      last_result = result;
    } 
  }
  return result;
}
