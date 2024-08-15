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
#include <stdexcept>


// #### Internal inclusions: ####
# include "../header/main.hpp"
# include "../header/game.hpp"
# include "../header/getkey.hpp"
# include "../header/tui.hpp"


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
        if (piece->isPawn() and (pos[1] == 7 or not pos[1]) and abs(pos[0] - piece->x()) == 1)
          {
            int promotion = tui.askPromotion();
            if (promotion)
            {
              piece = game.promote(piece, promotion);
              isOver = not game.move(piece, pos[0], pos[1], true);
            }
            tui.showMessage(game.turn() ? "Black's turn" : "White's turn");
            tui.show();
          }
        else
        {
          isOver = not game.move(piece, pos[0], pos[1]);
        }
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
  game.updateCheckList(piece, x, y);
  ++ game;
}


int newGame(TUI& tui, Game& game)
{
  clear();
  bool endOfGame = false;
  tui.show();
  while (not endOfGame)
  {
    run(game, tui);
    endOfGame = game.isMate();
    // mvprintw(0, 0, "N OF CHECK: %d", (int)game.checkList().size()); // DEBUG
  }
  if (game.drawBy50Moves())
  {
    tui.showMessage("Draw: 50 moves");
  }
  else if (game.drawByRepetition())
  {
    tui.showMessage("Draw: repetition");
  }
  else if (game.checkList().size())
  {
    tui.showMessage("Checkmate");
  }
  else
  {
    tui.showMessage("Draw: Stalemate");
  }
  getch(); //< Wait for user to quit
  return 0;
}


void reapplyColors(int signature)
{
  if (not signature)
  {
    return;
  }
  int pos = (signature - 1) << 1;
  short r1, g1, b1, r2, g2, b2, r3, g3, b3;
  color_content(47 + pos, &r1, &g1, &b1);
  color_content(48 + pos, &r2, &g2, &b2);
  color_content(37 + (signature == 1 or signature == 4), &r3, &g3, &b3);
  init_color(COLOR_WHITE, r1, g1, b1);
  init_color(COLOR_BLUE, r2, g2, b2);
  init_color(COLOR_RED, r3, g3, b3);
}


int menu()
{
  Game game;
  TUI tui(&game);
  int input, theme, startGame = false;
  while (true)
  {
    input = tui.getMenuOption();
    switch (input)
    {
      case 0:
      case 4:
        return 0;
      case 1:
        newGame(tui, game);
        break;
      case 3:
        theme = tui.getTheme();
        reapplyColors(theme);
        tui.writeTheme(theme - 1);
        break;
      case 2:
        startGame = tui.loadGame();
        if (startGame)
        {
          newGame(tui, game);
        }
        break;
      default:
        throw runtime_error("unexpected menu behavior");
    }
  }
}


int main()
{
  return menu();
}