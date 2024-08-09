/**
 * @file tui.cpp
 * @author JakeTheRealOne (jakelevrai@outook.be)
 * @brief Source file for the 
 * @version 0.1
 * @date 08-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Std inclusions: ####
# include <vector>
# include <string>
# include <unordered_set>
using namespace std;

// #### Ncurses inclusion: ####
# include <ncurses.h>

// #### Internal inclusions: ####
# include "../header/game.hpp"
# include "../header/piece.hpp"
# include "../header/getkey.hpp"
# include "../header/tui.hpp"


TUI::TUI()
{
  throw invalid_argument("TUI::TUI() is not available, please use TUI::TUI(Game* game) instead");
}


TUI::TUI(Game* game):
_game(game)
{
  initVars();
  initColors();
}


TUI::~TUI()
{
  endwin();
}


int TUI::x() const noexcept
{
  return _x;
}


int TUI::y() const noexcept
{
  return _y;
}


void TUI::initVars()
{
  ESCDELAY = 0;
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  start_color();
  keypad(stdscr, TRUE);

  _moves = vector<unordered_set<int>>(_game->SIZE, unordered_set<int>()); //< each set of _moves is a column

  getmaxyx(stdscr, _screenHeight, _screenWidth); //< Get screen size to center the board
  _xOffset = ((_screenWidth + 1) >> 1) - 8;
  _yOffset = ((_screenHeight + 1) >> 1) - 4;
}


void TUI::initColors()
{
  init_pair(1, COLOR_RED, COLOR_BLACK); // Cursor color
}


void TUI::show() const noexcept
{
  Piece* piece;
  // Board
  for (int i = 0; i < _game->SIZE; ++ i)
  {
    for (int j = 0; j < _game->SIZE; ++ j)
    {
      mvprintw(_yOffset + j, _xOffset + (i << 1), " %s", repr(i, j, false).c_str());
    }
  }
  // Cursor
  attron(COLOR_PAIR(1));
  mvprintw(_yOffset + _y, _xOffset + (_x << 1), " %s", repr(_x, _y, true).c_str());
  attroff(COLOR_PAIR(1));

}


void TUI::move(const int oldX, const int oldY, const int newX, const int newY) noexcept
{
  clearMoves();
  mvprintw(_yOffset + oldY, _xOffset + (oldX << 1), "  ");
  mvprintw(_yOffset + newY, _xOffset + (newX << 1), " %s", repr(newX, newY, false).c_str());
}


void TUI::showMoves(const int x, const int y) noexcept
{
  clearMoves();
  Piece* piece = _game->at(x, y);
  if (piece == nullptr)
  {
    return;
  }
  vector<vector<int>> moves = piece->read();
  for (const vector<int>& move : moves)
  {
    _moves[move[0]].insert(move[1]);
    mvprintw(_yOffset + move[1], _xOffset + (move[0] << 1), " .");
  }
}


void TUI::clearMoves() noexcept
{
  unordered_set<int> moves;
  for (int x = 0; x < _game->SIZE; ++ x)
  {
    moves = _moves[x];
    _moves[x].clear();
    for (const int y : moves)
    {
      mvprintw(_yOffset + y, _xOffset + (x << 1), " %s", repr(x, y, false).c_str());
    }
  }
}


void TUI::moveCursor(const int direction) noexcept
{
  if (direction < 0 or direction > 4)
  {
    return;
  }
  mvprintw(_yOffset + _y, _xOffset + (_x << 1), " %s", repr(_x, _y, false).c_str());
  switch (direction)
  {
    case 0:
      _y > 0 ? -- _y : 0;
      break;
    case 1:
      _y < _game->SIZE - 1 ? ++ _y : 0;
      break;
    case 2:
      _x > 0 ? -- _x : 0;
      break;
    case 3:
      _x < _game->SIZE - 1 ? ++ _x : 0;
      break;
  }
  attron(COLOR_PAIR(1));
  mvprintw(_yOffset + _y, _xOffset + (_x << 1), " %s", repr(_x, _y, true).c_str());
  attroff(COLOR_PAIR(1));
}


void TUI::showHelp(const bool type) const noexcept
{
  // TODO
}


void TUI::showMessage(const string& message) const noexcept
{
  if (message.size() > 16)
  {
    return; // Too long
  }
  mvprintw(_yOffset - 1, _xOffset, "%s", message.c_str());
}

string TUI::repr(const int x, const int y, const bool isCursor) const noexcept
{
  if (_moves[x].find(y) != _moves[x].end())
  {
    return ".";
  }
  Piece* piece = _game->at(x, y);
  if (piece == nullptr)
  {
    return isCursor ? "+" : " ";
  }
  return string(1, piece->repr());
}