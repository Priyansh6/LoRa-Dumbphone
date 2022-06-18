#ifndef KEYPAD_H
#define KEYPAD_H

#define NUM_ROWS 4
#define NUM_COLS 3
#define NUM_MODES 5
#define MODE_CHANGE_WAIT_TIME 2000
#define READ_DELAY 100

typedef struct key {
  char key_value;
  int row;
  int col;
} key_t;

// Initialises the gpio pins required for keypad
void init_keypad(void);

// Tries to read key with given mode and returns key with negative row and col
// if nothing is returned
key_t mode_read_key(int mode);

// Tries to read key from a single button and returns key when time elapsed 
// after last button press is greater than MODE_CHANGE_WAIT_TIME or different
// button is pressed. If no button is pressed, it returns '\0'
char read_key();

#endif