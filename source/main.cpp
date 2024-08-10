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
  tui.showMessage(game.turn() ? "Black's turn" : "White's turn");
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
        state = tui.showMoves(pos[0], pos[1]);
      }
    }
  }
  tui.move(x, y, piece->x(), piece->y());
  ++ game;
}

int executeGame()
{
  bool endOfGame = false;
  Game game;
  TUI tui(&game);
  tui.show();
  while (not endOfGame)
  {
    run(game, tui);
    endOfGame = game.isMate();
  }
  if (game.checkList().size())
  {
    tui.showMessage("Checkmate");
  }
  else
  {
    tui.showMessage("Stalemate");
  }
  return 0;
}

int main()
{
  return executeGame();
}