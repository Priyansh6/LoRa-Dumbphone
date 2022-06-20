#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#include "keypad.h"
#include "utilities.h"
#include "lora.h"

enum Win {CHAT_WIN, IN_WIN};

WINDOW *create_newwin(int height, int width, int starty, int startx);

int main(int argc, char **argv) {
  wiringPiSetup();
  init_keypad();

  initscr();
  refresh();

  int fd = init_lora();
  pq_t *pq = alloc_pq();
  message_t temp, display_message, writing_message;

  char *sender = argv[1];

  bool quit = false;

  enum Win active = IN_WIN;

  /* 1. Receive from specific sender
     2. Receive from all
     3. Set sender id
     4. Chat
     */

  int MAX_HEIGHT, MAX_WIDTH;
  float chat_scale;

  getmaxyx(stdscr, MAX_HEIGHT, MAX_WIDTH);
  chat_scale = 0.85;

  WINDOW *chat_win = create_newwin(
    MAX_HEIGHT * chat_scale,
    MAX_WIDTH,
    0,
    0);

  WINDOW *input_win = create_newwin(
    MAX_HEIGHT * (1 - chat_scale),
    MAX_WIDTH,
    MAX_HEIGHT * chat_scale,
    0);

  // wprintw("adam@|utc+1|22:58:11|01-10-2002|: Hamish is cool");
  // refresh();
  
  //sender@time|contents\0

  mvwprintw(input_win, 0, 0, "%s: msg1", sender);
  wrefresh(input_win);

  char key = \0;

  /*
  while(!quit) {
    poll_messages(fd, pq, &temp);

    while (!is_empty_pq(pq)) {
      display_message = pop_from_pq(pq);

      //pretty_print(display_message)
    }

    bool c = read_key(&key);

    if (c) {
      if (key == send_key) {
        send_message(fd, pq, writing_message);
      } else {
        // writing_message.t = ...
        // write char to current message/screen
      }
    } 
  }
  */

  getch();

  endwin();
  close_lora(fd);
  free_pq(pq);
  free(pq);

  return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win, *box_win;

  box_win = newwin(height, width, starty, startx);
	local_win = newwin(height - 2, width - 2, starty + 1, startx + 1);
	box(box_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(box_win);
  wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void write_message(message_t m) {
  break;
}

// time_t     now;
// struct tm  ts;
// char       buf[80];

// // Get current time
// time(&now);

// // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
// ts = *localtime(&now);
// strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
// printf("%s\n", buf);
// return 0;

// mvwprintw(chat_win, 0, 0, "%d", 0);
// for (int i = 1; i < 50; i++) {
//   mvwprintw(chat_win, i + 1, 1, "%d", i);
//   wrefresh(chat_win);
//   napms(100);
// }

// mvwprintw(chat_win, 0, 0, "%d", 0);
// int col = 0;
// for (int i = 1; i < 170; i++) {
//   mvwprintw(chat_win, 0, col, "%d", i);
//   wrefresh(chat_win);
//   col += log10(i) + 2;
//   napms(50);
// }

// wprintw(chat_win, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
// wrefresh(chat_win);

// mvwprintw(chat_win, 1, 0, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
// wrefresh(chat_win);