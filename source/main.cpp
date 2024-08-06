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


void initTui()
{
  ESCDELAY = 0;
  initscr();
  cbreak();
  noecho();
  scrollok(stdscr, TRUE); // debug
  start_color();
  keypad(stdscr, TRUE);

  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_color(1, 722, 529, 384); // Dark tile
  init_color(2, 929, 839, 690); // Light tile
  init_color(3, 722, 373, 373); // Selected

  init_pair(2, 1, COLOR_BLACK); // Board palette
  init_pair(3, 2, COLOR_BLACK); // Board palette
  init_pair(4, 3, COLOR_BLACK);
}


void run(Game& game)
{
  bool state = 0; //> State 0 = selecting a piece, 1 = moving a piece
  bool endOfRun = false;
  int input, x = 0, y = 0, showX, showY;
  while (not endOfRun)
  {
    state ? game.showMoves(showX, showY, x, y) : game.display(x, y);
    input = getkey();
    // clear();
    if (input == -1) // Ignore input
    {
      continue;
    }
    if (not input) // Go back
    {
      if (state)
      {
        printw("go back\n");
      }
      state = 0;
    }
    else if (input == 1)
    {
      printw("confirm %s\n", (state ? "move" : "selec"));
      if (state)
      {
        if (not game.move(game.at(showX, showY), x, y))
        {
          endOfRun = true;
        }
        state = false;
      } else {
        showX = x, showY = y;
        if (game.at(x, y) != nullptr and game.at(x, y)->player() == game.turn())
        {
          state = true;
        }
      }

    }
    else {
      // change direction
      printw("changing pos: OLD %d %d -> ", x, y);
      switch (input)
      {
        case 2:
          y -= (y > 0);
          break;
        case 3:
          y += (y < game.SIZE - 1);
          break;
        case 4:
          x -= (x > 0);
          break;
        case 5:
          x += (x < game.SIZE - 1);
          break;
      }
      printw("NEW %d %d\n", x, y);
    }
  }
  ++ game;
}


int main()
{
  initTui();
  Game game;
  bool isOver = false;
  while (not isOver)
  {
    run(game);
  }
  endwin();
  return 0;
}