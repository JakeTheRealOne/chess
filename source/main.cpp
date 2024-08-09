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
# include "../header/tui.hpp"

// #### Std inclusions: ####
# include <iostream>
using namespace std;


void initTui()
{
  ESCDELAY = 0;
  initscr();
  cbreak();
  noecho();
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
    clear();
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

vector<int> getPos(TUI& tui, bool abortFlag)
{
  int input;
  do {
    input = getkey();
    if (1 < input and input < 6)
    {
      tui.moveCursor(input - 2);
    }
  }
  while (input != 1 and not (abortFlag and not input));
  if (abortFlag and not input)
  {
    // Abort (ESC key pressed)
    return {};
  }
  else 
  {
    // Confirm (ENTER key pressed)
    return {tui.x(), tui.y()};
  }
}

void run(Game& game, TUI& tui)
{
  bool state = false, isOver = false;
  int x, y;
  Piece* piece;
  while (not isOver)
  {
    vector<int> pos = getPos(tui, state);
    if (state)
    {
      if (not pos.size())
      {
        tui.clearMoves();
        state = false;
      }
      else
      {
        x = piece->x(), y = piece->y();
        isOver = not game.move(piece, pos[0], pos[1]);
      }
    }
    else
    {
      piece = game.at(pos[0], pos[1]);
      if (piece != nullptr and piece->player() == game.turn())
      {
        tui.showMoves(pos[0], pos[1]);
        state = true;
      }
    }
  }
  tui.move(x, y, piece->x(), piece->y());
  ++ game;
}

int main()
{
  Game game;
  TUI tui(&game);
  tui.show();
  run(game, tui);
  int input = getkey();
  return 0;
}