/*  This program demonstrates the use of the 
    cbreak and nocbreak functions in the curses library.
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <curses.h>

int main(void)
{ char c, i;
  char s[64];

  initscr();   /* turn on curses functionality */

  nocbreak();  /* turn on line buffering on input */

  for (i=0; i<5; i++)
  {
    addstr("Type a character: "); refresh();
    c = getch();
    getch(); /* strip off LF */
    sprintf(s, "\n\r  c = %c = 0x%X\n\r", c, c);
    addstr(s); refresh();
  }

  addstr("Press [Enter] to Continue..."); refresh();
  getchar();
  puts("\r");

  cbreak();  /* turn off line buffering on input */

  for (i=0; i<5; i++)
  {
    addstr("Enter a character: "); refresh();
    c = getch();
    sprintf(s, "\n\r  c = %c = 0x%X\n\r", c, c);
    addstr(s); refresh();
  }

  addstr("Press [Enter] to Quit..."); refresh();
  getchar();

  endwin();    /* turn off curses functionality */
}

