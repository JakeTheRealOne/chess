/**
 * @file main.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief main Source file
 * @version 0.1
 * @date 29-07-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Ncurses inclusion: ####
# include <ncurses.h>


// #### Internal inclusions: ####
# include "../header/main.hpp"
# include "../header/game.hpp"
# include "../header/getkey.hpp"

// #### Std inclusions: ####
# include <iostream>
using namespace std;


void clearTerminal()
{
  system("clear");
}


void initTui()
{
  ESCDELAY = 0;
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
}


void run(Game& game)
{
  bool state = 0; //> State 0 = selecting a piece, 1 = moving a piece
  bool endOfRun = false;
  int input;
  while (not endOfRun)
  {
    input = getkey();
  }
}


int main()
{
  initTui();
  Game game;
  run(game);
  endwin();
  return 0;
}