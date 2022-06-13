/*  This program demonstrates the use of the 
    curses library.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#define KB_UP      65
#define KB_DN      66
#define KB_RIGHT   67
#define KB_LEFT    68

#define ROWS 15
#define COLS 15

char strMap[ROWS][COLS+1];
char ShipMap[ROWS][COLS];
//char strCursor[] = "> <";
//char strCursor2[] = "^";

#define cHIT     'X'
#define cSUNK    'S'
#define cNOT_HIT 'O'
#define cMISS    '~'

#define SHIPS 6
#define MAXSIZE 6
char ShipLen[SHIPS] =
{ 2, 3, 4, 5, 5, 6
};

enum eOrientation
{ UP_DOWN, RIGHT_LEFT
} Orientation[SHIPS];
char ShipCoordR[SHIPS][MAXSIZE];
char ShipCoordC[SHIPS][MAXSIZE];
int ShipSunk[SHIPS];
int SunkShips;

#define BOMBS ((ROWS * COLS) / 2)
int Bombs;

/*********************************************/
void InitScreen(void)
{ int row, col;

  printf("Use Arrow Keys to Position to Cursor.\n");
  printf("Press 'D' to drop a Bomb.\n");
  printf("Press [Enter] to continue...\n");
  getchar();

  initscr();      /* turn on curses functionality */
  clear();        /* clear the screen     */
  nodelay(stdscr, 1); /* no blocking      */
  noecho();      /* don't echo characters */

  for (row=0; row<ROWS; row++)
  { memset(&strMap[row][0], ' ', COLS);
    strMap[row][COLS] = '\0';
  }
}
/*****************************************************/
void ClearNewShip(int ship)
{ int i;

  for (i=0; i<ShipLen[ship]; i++)
  { ShipCoordR[ship][i] = 0;
    ShipCoordC[ship][i] = 0;
  }
}
/*****************************************************/
void InitBattleships(void)
{ int ship, i, start_r, start_c, row, col;
  int GoodPlacement;

  srand(time(0));

  /* Initialize ShipMap */
  for (row=1; row<=ROWS; row++)
  { for (col=1; col<=COLS; col++)
	{ ShipMap[row][col] = 0;
	}
  }
  SunkShips = 0;
  Bombs = BOMBS;

  /* Place Ships */
  for (ship=0; ship<SHIPS; ship++)
  { ShipSunk[ship] = FALSE;

    Orientation[ship] = rand() % 2; /* UP_DOWN or LEFT_RIGHT */

    do
    { ClearNewShip(ship);
      if (Orientation[ship] == UP_DOWN)
      { start_r = rand() % (ROWS - 2 - ShipLen[ship]) + 1;
        start_c = rand() % (COLS - 2) + 1;
        for (i=0; i<ShipLen[ship]; i++)
        { ShipCoordR[ship][i] = start_r + i;
          ShipCoordC[ship][i] = start_c;
		}
	  }
	  else
	  { start_r = rand() % (ROWS - 2);
	    start_c = rand() % (COLS - 2 - ShipLen[ship]);
	    for (i=0; i<ShipLen[ship]; i++)
	    { ShipCoordR[ship][i] = start_r;
	      ShipCoordC[ship][i] = start_c + i;
		}
	  }
	  /* Check if new ship overlaps old ships */
      GoodPlacement = TRUE;
      if (ship > 0) /* Check Previous Ship Placements */
      { for (i=0; i<ShipLen[ship]; i++)
        { if (ShipMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] != 0)
          { GoodPlacement = FALSE;
            break;
          }
        }
      }
      if (GoodPlacement) /* Save new ship position */
      { for (i=0; i<ShipLen[ship]; i++)
        { ShipMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] = cNOT_HIT;
        }
      }
    } while (!GoodPlacement);
  } /* Next ship */
}
/*****************************************************/
void UpdateScreen(int CurR, int CurC)
{ int row, col;
  char str[16];

  for (row=0; row<ROWS; row++)
  { move(row, 0);
    addstr(strMap[row]);
  }
  /* Show Plane Cursor */
/*
  move(CurR, CurC-1);  addstr(strCursor);
  move(CurR+1, CurC);  addstr(strCursor2);
*/
  sprintf(str,"Bombs Left: %3d", Bombs);
  move(ROWS + 1, 3); addstr(str); 
  move(CurR, CurC); refresh();
}
/*****************************************************/
void MarkSunkShips(int CurR, int CurC)
{ int ship, i;
  int Sunk;

  for (ship=0; ship<SHIPS; ship++)
  { if (!ShipSunk[ship])
    { Sunk = TRUE;
	  for (i=0; i<ShipLen[ship]; i++)
      { if (strMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] != cHIT)
        { Sunk = FALSE;
	      break;
        }
      }
      if (Sunk)
      { ShipSunk[ship] = TRUE;
        for (i=0; i<ShipLen[ship]; i++)
        { strMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] = cSUNK;
        }
        if (++SunkShips >= SHIPS)
        { UpdateScreen(CurR, CurC);
		  move(0, 2); addstr("YOU WIN!--YOU WIN!--YOU WIN!"); refresh(); 
          getchar();
          endwin();
          exit(0);
        }
      }
    }
  }
}
/*****************************************************/
void ShowUnsunkShips(void)
{ int ship, i;

  for (ship=0; ship<SHIPS; ship++)
  { if (!ShipSunk[ship])
    { for (i=0; i<ShipLen[ship]; i++)
      { if (ShipMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] == cNOT_HIT)
        { strMap[ShipCoordR[ship][i]][ShipCoordC[ship][i]] = cNOT_HIT;
        }
      }
    }
  }
//  refresh();
}

/*****************************************************/
int main(void)
{ int CurR, CurC;
  char c;

  InitScreen();

  InitBattleships();

  CurR = CurC = 0; /* initialize cursor to upper left */
  
  do
  { UpdateScreen(CurR, CurC);

    c = getch();  /* get input from user */

    switch (c)
    { case 27:
        if (getch() == 91)
        switch (getch())
        { case KB_UP: if (--CurR < 0) CurR = 0;
            break;
          case KB_DN: if (++CurR >= ROWS-1) CurR = ROWS - 1;
            break;
          case KB_LEFT:  if (--CurC < 0) CurC = 0;
            break;
          case KB_RIGHT: if (++CurC >= COLS-1) CurC = COLS - 1;
            break;
        }
        break;

      case 'd':
      case 'D':
        if (ShipMap[CurR][CurC])
        { strMap[CurR][CurC] = cHIT;
          MarkSunkShips(CurR, CurC);
        }
        else strMap[CurR][CurC] = cMISS;
      	if (--Bombs == 0)
        { 
          ShowUnsunkShips();
          UpdateScreen(CurR, CurC);
          move(0, 2); addstr("YOU LOSE--YOU LOSE--YOU LOSE"); refresh();
		  getchar();
          endwin();
          exit(1);
        }
      	break;

      case 'q':
//        getch();
        endwin();  /* turn off curses functionality */
        exit(0);
    }
    refresh();
  } while (1);
}
