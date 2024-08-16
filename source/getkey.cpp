/**
 * @file getkey.cpp
 * @author your name (you@domain.com)
 * @brief Source file for the getkey script
 * @version 0.1
 * @date 04-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// /!\ Prereq: ncurses library (-lncurses flag in g++)


// #### Ncurses inclusion: ####
# include <ncurses.h>

// #### Internal inclusions: ####
# include "../header/getkey.hpp"


int getkey()
{
  int input = getch();
  switch (input)
  {
    case 107: //< k
    case KEY_UP:
      return 2;
    case 106: //< j
    case KEY_DOWN:
      return 3;
    case 104: //< h
    case KEY_LEFT:
      return 4;
    case 108: //< l
    case KEY_RIGHT:
      return 5;
    case 10: //> Keyboard
    case KEY_ENTER: //> Keypad
      return 1;
    case 113: // q
    case 27: // esc
      return 0;
    default:
      return -1;      
  }
}
