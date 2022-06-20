#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdbool.h>

#define NUM_ROWS 4
#define NUM_COLS 3
#define NUM_MODES 5
#define BUTTON_WAIT_TIME 2000

typedef struct button {
  char value;
  int row;
  int col;
} button_t;

// Initialises the gpio pins required for keypad
void init_keypad(void);

// Tries to read key with given mode and returns key with negative row and col if nothing is
// returned. If mode is too high, it gets set to 0
button_t mode_read_key(int *mode);

// Tries to read key once from start_time milliseconds (obtained by calling millis())
//
// mid_key is set to the key read if a button is pressed. start_time is set to the time of the
// last button press. Returns true iff time has ran out or different button is pressed.
// If time has run out but no button was pressed, mid_key is set to '\0'
bool read_key_mid(char *mid_key, int *start_time);

// Tries to read key from a single button and returns key when ms elapsed after last button press
// is greater than BUTTON_WAIT_TIME or different button is pressed. If no button is pressed, it 
// returns '\0'
char read_key();

#endif
