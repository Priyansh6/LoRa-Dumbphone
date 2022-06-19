#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "keypad.h"
#include "utilities.h"

int main(void) {
  wiringPiSetup();
  init_keypad();

  initscr();

  // addstr("\n\n\n\n      WELCOME MESSAGE     \n\n\n\n");
  // refresh();

  // addstr("      HELP MESSAGE        \n\n\n\n");
  // refresh();

  bool quit = false;

  /* 1. Receive from specific sender
     2. Receive from all
     3. Set sender id
     4. Chat
     */

  int MAX_X, MAX_Y;

  getmaxyx(stdscr, MAX_X, MAX_Y);

  printw("x: %d | y: %d", MAX_X, MAX_Y);
  refresh();

  /* Window creation */
  /* utc00:00:00|01/01/2022|user1> Blah blah blah*/

  while(!quit) {
    break;
  }

  addstr("\npress any key to exit...");
  refresh();

  getch();

  endwin();

  return 0;
}