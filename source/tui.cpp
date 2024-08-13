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
#include <stdexcept>
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


void TUI::setGame(Game* newGame)
{
  if (newGame == nullptr)
  {
    throw invalid_argument("TUI::setGame needs an initialized game pointer");
  }
  _game = newGame;
}

void TUI::initVars() noexcept
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


void TUI::initColors() noexcept
{
  // OLD SCHOOL THEME:
  /*
  init_color(COLOR_WHITE, 823, 705, 549);
  init_color(COLOR_BLUE, 450, 290, 184);
  */

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

  init_pair(9, COLOR_BLACK, COLOR_WHITE); // Promotion panel
  init_pair(10, COLOR_WHITE, -1);
}


void TUI::resizeWindow(int sig) noexcept
{
  // TODO
}

void TUI::computeScreenSize() noexcept
{
  static int x = 0;
  getmaxyx(stdscr, _screenHeight, _screenWidth); //< Get screen size to center the board
  _xOffset = ((_screenWidth + 1) >> 1) - 8;
  _yOffset = ((_screenHeight + 1) >> 1) - 4;
  x += 5;
}


void TUI::show() const noexcept
{
  // Board
  for (int i = 0; i < _game->SIZE; ++ i)
  {
    for (int j = 0; j < _game->SIZE; ++ j)
    {
      update(i, j);
    }
  }
  // Cursor
  update(_x, _y, true);
}


void TUI::move(const int oldX, const int oldY, const int newX, const int newY) noexcept
{
  clearMoves();
  update(oldX, oldY);
  update(newX, newY, true);
  // En passant
  if (_game->at(newX, newY)->isPawn() and newX != oldX)
  {
    update(newX, oldY);
  }
}


bool TUI::showMoves(const int x, const int y) noexcept
{
  clearMoves();
  Piece* piece = _game->at(x, y);
  if (piece == nullptr)
  {
    return 0;
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
  return (bool)moves.size();
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
  update(_x, _y, true); //< Reprint the cursor if it was deleted by the above for loop
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


void TUI::showHelp(const short type) const noexcept
{
  // TODO
}


void TUI::showLogo() const noexcept
{
  int yOffset = _yOffset - LOGO_HEIGHT - 1, xOffset = _xOffset - ((LOGO_WIDTH - (_game->SIZE << 1)) >> 1) - 1, rowIndex = 0;
  for (const string& row : LOGO)
  {
    mvprintw(yOffset + rowIndex, xOffset, "%s", row.c_str());
    ++ rowIndex;
  }
}


void TUI::showMenu() const noexcept
{
  clear();
  showLogo();
  for (int index = 0; index < 4; ++ index)
  {
    short pair = 9 + (bool)index;
    attron(COLOR_PAIR(pair));
    mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", MENU_OPTIONS[index].c_str());
    attroff(COLOR_PAIR(pair));
  }
}


void TUI::showMessage(const string& message) const noexcept
{
  if (message.size() > 16 or _yOffset <= 0 or _xOffset < 0)
  {
    return; // Too long
  }
  mvprintw(_yOffset - 1, _xOffset, "%s", message.c_str());
  mvprintw(_yOffset - 1, _xOffset + message.size(), "%s", string(16 - message.size(), ' ').c_str());
}


void TUI::showPromotionPanel() const noexcept
{
  clear();
  showMessage("Promotion panel");

  for (int index = 0; index < 4; ++ index)
  {
    short pair = 9 + (bool)index;
    attron(COLOR_PAIR(pair));
    mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", PROMOTION_OPTIONS[index].c_str());
    attroff(COLOR_PAIR(pair));
  }
}


int TUI::changePromotion(int index, int increment) const noexcept
{
  if (index + increment < 0 or index + increment >= PROMOTION_OPTIONS.size())
  {
    return index; // Abort
  }

  attron(COLOR_PAIR(10));
  mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", PROMOTION_OPTIONS[index].c_str());
  attroff(COLOR_PAIR(10));

  index += increment;

  attron(COLOR_PAIR(9));
  mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", PROMOTION_OPTIONS[index].c_str());
  attroff(COLOR_PAIR(9));

  return index;
}


int TUI::askPromotion() const noexcept
{
  int input, menuIndex = 0;
  showPromotionPanel();
  do
  {
    input = getkey();
    if (input == 2 or input == 3)
    {
      menuIndex = changePromotion(menuIndex, input == 2 ? -1 : 1);
    }
  }
  while (input and input != 1);
  return (bool)input * (menuIndex + 1); // TODO
}


int TUI::getMenuOption() const noexcept
{
  int input, menuIndex = 0;
  showMenu();
  do
  {
    input = getkey();
    if (input == 2 or input == 3)
    {
      menuIndex = changeMenu(menuIndex, input == 2 ? -1 : 1);
    }
  }
  while (input and input != 1);
  return (bool)input * (menuIndex + 1); // TODO
}


int TUI::changeMenu(int index, int increment) const noexcept
{
  if (index + increment < 0 or index + increment >= MENU_OPTIONS.size())
  {
    return index; // Abort
  }

  attron(COLOR_PAIR(10));
  mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", MENU_OPTIONS[index].c_str());
  attroff(COLOR_PAIR(10));

  index += increment;

  attron(COLOR_PAIR(9));
  mvprintw(_yOffset + (index << 1) + 1, _xOffset, "%s", MENU_OPTIONS[index].c_str());
  attroff(COLOR_PAIR(9));

  return index;
}


void TUI::update(const int x, const int y, const bool isCursor) const noexcept
{
  if (_yOffset + y < 0 or _xOffset + (x << 1) < 0)
  {
    return; // Abort (out of bound)
  }

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