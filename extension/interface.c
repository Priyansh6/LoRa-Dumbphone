#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "keypad.h"
#include "utilities.h"

WINDOW *create_newwin(int height, int width, int starty, int startx);

int main(void) {
  wiringPiSetup();
  init_keypad();

  initscr();
  refresh();

  bool quit = false;

  enum Win {CHAT_WIN = 0, IN_WIN = 1};

  /* 1. Receive from specific sender
     2. Receive from all
     3. Set sender id
     4. Chat
     */

  int MAX_HEIGHT, MAX_WIDTH;
  int chat_start_x, chat_start_y, input_start_x, input_start_y;
  float chat_scale;

  getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
  chat_scale = 0.85;

  WINDOW *chat_win = create_newwin(
    MAX_HEIGHT * chat_scale,
    MAX_WIDTH,
    0,
    0);

  chat_start_y = 1;
  chat_start_x = 1;

  WINDOW *input_win = create_newwin(
    MAX_HEIGHT * (1 - chat_scale),
    MAX_WIDTH,
    MAX_HEIGHT * chat_scale,
    0);

  input_start_y = (MAX_HEIGHT * chat_scale) + 1;
  input_start_x = 1;  

  // move(input_start_y, input_start_x);
  // printw("adam@|utc+1|22:58:11|01-10-2002|: Hamish is cool");
  // refresh();

  int cur_y, cur_x;

  while(!quit) {
    getyx(cur_y, cur_x);

    
    break;
  }

  getch();

  endwin();

  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}