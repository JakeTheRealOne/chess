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
  use_default_colors();

  _moves = vector<unordered_set<int>>(_game->SIZE, unordered_set<int>()); //< each set of _moves is a column

  computeScreenSize();
}


void TUI::initColors()
{
  // OLD SCHOOL THEME:
  /*init_color(COLOR_WHITE, 823, 705, 549);
  init_color(COLOR_BLUE, 450, 290, 184);*/
  // HIKARU THEME:
  init_color(COLOR_WHITE, 1000, 1000, 1000);
  init_color(COLOR_BLUE, 615, 764, 878);

  init_color(COLOR_RED, 933, 360, 349);
  init_pair(1, COLOR_RED, COLOR_WHITE); // Cursor color
  init_pair(2, COLOR_RED, COLOR_BLUE);

  init_color(COLOR_YELLOW, 654, 498, 411);
  init_pair(3, COLOR_YELLOW, COLOR_WHITE); // White pieces
  init_pair(4, COLOR_YELLOW, COLOR_BLUE);

  init_color(COLOR_BLACK, 0, 0, 0);
  init_pair(5, COLOR_BLACK, COLOR_WHITE); // Black pieces
  init_pair(6, COLOR_BLACK, COLOR_BLUE);

  init_pair(7, -1, COLOR_WHITE); // Default color
  init_pair(8, -1, COLOR_BLUE);
}


void TUI::computeScreenSize() noexcept
{
  getmaxyx(stdscr, _screenHeight, _screenWidth); //< Get screen size to center the board
  _xOffset = ((_screenWidth + 1) >> 1) - 8;
  _yOffset = ((_screenHeight + 1) >> 1) - 4;
}


void TUI::show() const noexcept
{
  bool paintFlag = 0; 
  // Board
  for (int i = 0; i < _game->SIZE; ++ i)
  {
    for (int j = 0; j < _game->SIZE; ++ j)
    {
      update(i, j);
      paintFlag = not paintFlag;
    }
    paintFlag = not paintFlag;
  }
  // Cursor
  update(_x, _y, true);
}


void TUI::move(const int oldX, const int oldY, const int newX, const int newY) noexcept
{
  clearMoves();
  update(oldX, oldY);
  update(newX, newY);
  // En passant
  if (_game->at(newX, newY)->isPawn() and newX != oldX)
  {
    update(newX, oldY);
  }
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
  }
  for (const vector<int>& move : moves)
  {
    update(move[0], move[1]);
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
      update(x, y);
    }
  }
}


void TUI::moveCursor(const int direction) noexcept
{
  if (direction < 0 or direction > 4)
  {
    return;
  }
  update(_x, _y);
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
  update(_x, _y, true);
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

void TUI::update(const int x, const int y, const bool isCursor) const noexcept
{
  short colorPair = 1 + ((x + y) % 2);
  string content;

  if (_moves[x].find(y) != _moves[x].end())
  {
    content = isCursor ? "+" : ".";
  }
  else if (_game->at(x, y) == nullptr)
  {
    colorPair = isCursor ? colorPair : 7 + ((x + y) % 2);
    content = isCursor ? "+" : " ";
  }
  else
  {
    colorPair = isCursor ? colorPair : 3 + ((x + y) % 2) + (_game->at(x, y)->player() << 1);
    content = string(1, _game->at(x, y)->repr());
  }
  attron(COLOR_PAIR(colorPair));
  mvprintw(_yOffset + y, _xOffset + (x << 1), " %s", content.c_str());
  attroff(COLOR_PAIR(colorPair));
}