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

# include <iostream>
using namespace std;

// #### Internal inclusions: ####
# include "../header/getkey.hpp"


int getkey()
{
  int input = getch();
  switch (input)
  {
    case KEY_UP:
      return 2;
    case KEY_DOWN:
      return 3;
    case KEY_LEFT:
      return 4;
    case KEY_RIGHT:
      return 5;
    case 10: //> Keyboard
    case KEY_ENTER: //> Keypad
      return 1;
    case 27:
      return 0;
    default:
      return -1;      
  }
}
