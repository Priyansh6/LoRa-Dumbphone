#include <math.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "keypad.h"
#include "utilities.h"
#include "lora.h"


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

void write_message(message_t *m, char key, bool pressed, WINDOW *input_win) {
  int str_len = strlen(m->contents);  


  if (key == '*' && str_len >= 1){
    m->contents[str_len - 1] = '\0';
    key = '\0';
  }else if (pressed) {
    m->contents[str_len] = key; 
    m->contents[str_len + 1] = '\0';
    key = '-';
    // add key to the string and set it to the singleton
  }

  if (key == '\0'){
    mvwprintw(input_win, 0, 0, "%s>%s ", m->sender, m->contents);
    int y, x;
    getyx(input_win, y, x);
    wmove(input_win, y, x - 1);
  } else if (key == '-') {
    mvwprintw(input_win, 0, 0, "%s>%s", m->sender, m->contents);
  } else if (key != '*') {
    mvwprintw(input_win, 0, 0, "%s>%s%c", m->sender, m->contents, key);
  }

  wrefresh(input_win);

}


void receive_message(message_t *m, char key, bool pressed, WINDOW *input_win) {

  char buff[20];
  struct tm * timeinfo;
  timeinfo = localtime (&mtime);
  strftime(buff, sizeof(buff), "%b %d %H:%M", timeinfo)

  wrefresh(input_win);

}

int main(int argc, char **argv) {
  wiringPiSetup();
  init_keypad();

  initscr();
  refresh();

  //int fd = init_lora();
  //pq_t *pq = alloc_pq();
  message_t temp, display_message, writing_message;

  char *sender = argv[1];

  bool quit = false;

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

  scrollok(chat_win, true);

  // wprintw("adam@|utc+1|22:58:11|01-10-2002|: Hamish is cool");
  // refresh();
  
  //sender@time|contents\0

  message_t m;
  char key = '\0';


  strcpy(m.contents, "\0");
  strcpy(m.sender, sender);
  write_message(&m, '\0', false, input_win);

  
  int t = millis();
  
  while(!quit) {
    //poll_messages(fd, pq, &temp);

    //while (!is_empty_pq(pq)) {
      //display_message = pop_from_pq(pq);

      //pretty_print(display_message)
    //}

    bool c = read_key_mid(&key, &t);

    if (c) {
      t = millis();
    }
    
    if (key == '#'){
      wclear(input_win);
      wrefresh(input_win);
      strcpy(m.contents, "\0");
      strcpy(m.sender, sender);
      key = '\0';
    } else {
      write_message(&m, key, c, input_win);  
    }
    
    if (c || key == '*' || key == '#'){
        key = '\0';
        t = millis();
      }
  }

  getch();
  endwin();

  //close_lora(fd);
  //free_pq(pq);
  //free(pq);

  return 0;
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