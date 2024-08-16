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
# include <stdexcept>
# include <vector>
# include <string>
# include <unordered_set>
# include <fstream>
# include <filesystem>
using namespace std;
namespace fs = filesystem;

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
  delete _game;
  _game = nullptr;
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


Game* TUI::game() const noexcept
{
  return _game;
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

  // Themes
  init_color(47, 823, 705, 549); //< Classic
  init_pair(47, -1, 47);
  init_color(48, 450, 290, 184);
  init_pair(48, -1, 48);
  init_color(49, 1000, 1000, 1000); //< Glass
  init_pair(49, -1, 49);
  init_color(50, 615, 764, 878);
  init_pair(50, -1, 50);
  init_color(51, 910, 902, 882); //< Tournament
  init_pair(51, -1, 51);
  init_color(52, 188, 384, 275);
  init_pair(52, -1, 52);
  init_color(53, 769, 737, 651); //< Rich
  init_pair(53, -1, 53);
  init_color(54, 416, 400, 380);
  init_pair(54, -1, 54);

  // Cursor color
  init_color(37, 647, 78, 212);
  init_color(38, 267, 78, 647);

  readTheme();

  init_pair(1, COLOR_RED, COLOR_WHITE); // Cursor color
  init_pair(2, COLOR_RED, COLOR_BLUE);

  init_color(COLOR_YELLOW, 637, 498, 367);
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
  // Castling
  else if (_game->at(newX, newY)->isKing() and abs(newX - oldX) == 2)
  {
    update(newX - oldX == -2 ? 0 : _game->SIZE - 1, newY);
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
  attron(COLOR_PAIR(10));
  int yOffset = _yOffset - LOGO_HEIGHT - 1, xOffset = _xOffset - ((LOGO_WIDTH - (_game->SIZE << 1)) >> 1) - 1, rowIndex = 0;
  for (const string& row : LOGO)
  {
    mvprintw(yOffset + rowIndex, xOffset, "%s", row.c_str());
    ++ rowIndex;
  }
  attroff(COLOR_PAIR(10));
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


void TUI::showThemes() const noexcept
{
  clear();
  showLogo();

  int offset = _xOffset - 1, row = 0, col = 0;
  for (int index = 0; index < THEMES; ++ index)
  {
    short theme = 47 + (index << 1);
    attron(COLOR_PAIR(theme + 1));
    mvprintw(_yOffset + (row << 2), offset + 2 + (col * 5), "  ");
    mvprintw(_yOffset + 1 + (row << 2), offset + (col * 5), "  ");
    attroff(COLOR_PAIR(theme + 1));

    attron(COLOR_PAIR(theme));
    mvprintw(_yOffset + (row << 2), offset + (col * 5), "  ");
    mvprintw(_yOffset + 1 + (row << 2), offset + 2 + (col * 5), "  ");
    attroff(COLOR_PAIR(theme));
    ++ col;
    if (col == 4)
    {
      ++ row;
      col = 0;
    }
  }
  // Display selector:
  attron(COLOR_PAIR(9));
  mvprintw(_yOffset + 2, offset + (_theme * 5), "    ");
  attroff(COLOR_PAIR(9));
}


void TUI::showMessage(const string& message) const noexcept
{
  if (message.size() > 16 or _yOffset <= 0 or _xOffset < 0)
  {
    return; // Too long
  }
  attron(COLOR_PAIR(10));
  mvprintw(_yOffset - 1, _xOffset, "%s", message.c_str());
  mvprintw(_yOffset - 1, _xOffset + message.size(), "%s", string(16 - message.size(), ' ').c_str());
  attroff(COLOR_PAIR(10));
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


int TUI::getTheme() const noexcept
{
  int input, menuIndex = _theme;
  showThemes();
  do
  {
    input = getkey();
    if (input > 1 and input < 6)
    {
      menuIndex = changeTheme(menuIndex, input % 2 ? 1 : -1, input > 3);
    }
  }
  while (input and input != 1);
  return (bool)input * (menuIndex + 1); // TODO
}


int TUI::changeTheme(int index, int increment, bool orientation) const noexcept
{
  int newIndex = index + (orientation ? 1 : 4 ) * increment, offset = _xOffset - 1;
  if (0 <= newIndex and newIndex < THEMES)
  {
    mvprintw(_yOffset + 2 + (index / 4), offset + ((index % 4) * 5), "    ");
    attron(COLOR_PAIR(9));
    mvprintw(_yOffset + 2 + (newIndex / 4), offset + ((newIndex % 4) * 5), "    ");
    attroff(COLOR_PAIR(9));
    return newIndex;
  }
  else
  {
    return index;
  }
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
    attron(A_BOLD);
    colorPair = isCursor ? colorPair : 3 + ((x + y) % 2) + (_game->at(x, y)->player() << 1);
    content = string(1, _game->at(x, y)->repr());
  }
  attron(COLOR_PAIR(colorPair));
  mvprintw(_yOffset + y, _xOffset + (x << 1), " %s", content.c_str());
  attroff(COLOR_PAIR(colorPair));
  attroff(A_BOLD);
}


int TUI::readTheme()
{
  ifstream file("memory/theme", ios::binary);
  if (!file)
  {
    return 1;
  }
  char theme;
  file.read(&theme, sizeof(theme));
  _theme = (int)theme;
  int pos = (theme) << 1;
  short r1, g1, b1, r2, g2, b2, r3, g3, b3;
  color_content(47 + pos, &r1, &g1, &b1);
  color_content(48 + pos, &r2, &g2, &b2);
  color_content(37 + (not theme or theme == 3), &r3, &g3, &b3);
  init_color(COLOR_WHITE, r1, g1, b1);
  init_color(COLOR_BLUE, r2, g2, b2);
  init_color(COLOR_RED, r3, g3, b3);
  file.close();
  return 0;
}


int TUI::writeTheme(char theme)
{
  if (theme == -1)
  {
    return 1;
  }
  ofstream file("memory/theme", ios::binary);
  if (!file)
  {
    return 1;
  }
  _theme = (int)theme;
  file.write(&theme, sizeof(theme));
  return 0;
}


int TUI::showLoadMenu()
{
  _saveFiles.clear();
  clear();
  showLogo();
  fs::directory_iterator savedGames("memory/saved_games");
  int index = 0, moves, pos = _yOffset;
  for (fs::directory_entry entry : savedGames)
  {
    if (index >= 6)
    {
      break; //< Prevent graphical bugs
    }
    moves = readFile(entry.path());
    if (moves != -1)
    {
      _saveFiles.push_back(entry.path());
      attron(COLOR_PAIR(10));
      mvprintw(pos, _xOffset, "%s", cut16(entry.path()).c_str());
      attroff(COLOR_PAIR(10));
      mvprintw(pos + 1, _xOffset, "%d moves played", (unsigned char)moves);
      ++ index;
      pos += 3;
    }
  }
  if (index == 0)
  {
    mvprintw(_yOffset, _xOffset, "No save files found");
  }
  else {
    attron(COLOR_PAIR(9));
    mvprintw(_yOffset, _xOffset, "%s", cut16(_saveFiles[0]).c_str());
    attroff(COLOR_PAIR(9));
  }
  return index;
}


int TUI::changeLoad(int index, int increment, bool orientation) const noexcept
{
  int newIndex = index + increment;
  if (newIndex >= 0 and newIndex < _saveFiles.size())
  {
    attron(COLOR_PAIR(10));
    mvprintw(_yOffset + (index * 3), _xOffset, "%s", cut16(_saveFiles[index]).c_str());
    attroff(COLOR_PAIR(10));

    attron(COLOR_PAIR(9));
    mvprintw(_yOffset + (newIndex * 3), _xOffset, "%s", cut16(_saveFiles[newIndex]).c_str());
    attroff(COLOR_PAIR(9));

    return newIndex;
  }
  else
  {
    return index;
  }
}


int TUI::loadGame()
{
  int option, index, input;
  while (true)
  {
    showLoadMenu();
    index = 0;
    do
    {
      input = getkey();
      if (input == 2 or input == 3)
      {
        index = changeLoad(index, input == 2 ? -1 : +1, 0);
      }
    }
    while (input and input != 1);
    if (_saveFiles.empty() or not input)
    {
      return 0;
    }
    showLoadOptions(index);
    option = getLoadOption();
    if (option < 2)
    {
      break;
    }
  }
  if (option)
  {
    fs::remove(_saveFiles[index]);
  }
  else
  {
    delete _game;
    _game = new Game(_saveFiles[index]);
  }
  return not option;
}


int TUI::readFile(string path) const
{
  // Magic bytes:
  static char expected[9] = {'C', 'H', 'E', 'S', 'S', 'J', 'K', 'L', 'V'};
  ifstream file(path, ios::binary);
  if (!file)
  {
    return -1;
  }
  char signature[9];
  file.read(signature, sizeof(signature));
  if (not equal(begin(expected), end(expected), begin(signature)))
  {
    return -1;
  }
  char moves;
  file.read(&moves, sizeof(moves));
  file.close();
  return (int)moves;
}


string TUI::cut16(fs::path path) const
{
  string output = path.filename();
  if (output.size() < 16)
  {
    output.reserve(16);
    output.append(string(16 - output.size(), ' '));
  }
  else if (output.size() > 16)
  {
    output.erase(output.begin() + 13, output.end());
    output.append("...");
  }
  return output;
}


void TUI::showLoadOptions(int index)
{
  clear();
  showLogo();
  attron(COLOR_PAIR(10));
  mvprintw(_yOffset, _xOffset, "%s", cut16(_saveFiles[index]).c_str());
  attroff(COLOR_PAIR(10));
  for (int option = 0; option < 3; ++ option)
  {
    attron(COLOR_PAIR(9 + (bool)option));
    mvprintw(_yOffset + ((option + 1) << 1), _xOffset, "%s", LOAD_OPTIONS[option].c_str());
    attroff(COLOR_PAIR(9 + (bool)option));
  }
}


int TUI::getLoadOption()
{
  int input, index = 0;
  do
  {
    input = getkey();
    if (input == 2 or input == 3)
    {
      index = changeLoadOption(index,input == 2 ? -1 : +1);
    }
  }
  while (input and input != 1);
  return (not input ? 2 : index);
}


int TUI::changeLoadOption(int index, int increment)
{
  int newIndex = index + increment;
  if (newIndex >= 0 and newIndex < 3)
  {
    attron(COLOR_PAIR(10));
    mvprintw(_yOffset + ((index + 1) << 1), _xOffset, "%s", LOAD_OPTIONS[index].c_str());
    attroff(COLOR_PAIR(10));

    attron(COLOR_PAIR(9));
    mvprintw(_yOffset + ((newIndex + 1) << 1), _xOffset, "%s", LOAD_OPTIONS[newIndex].c_str());
    attroff(COLOR_PAIR(9));

    return newIndex;
  }
  else
  {
    return index;
  }
}


int TUI::askName()
{
  attron(COLOR_PAIR(9));
  mvprintw(_yOffset + 2, _xOffset, "%s", string(16,' ').c_str());
  attroff(COLOR_PAIR(9));
  char input[30];
  attron(COLOR_PAIR(10));
  mvprintw(_yOffset, _xOffset, "Enter game name:");
  attroff(COLOR_PAIR(10));
  echo();
  attron(COLOR_PAIR(9));
  ::move(_yOffset + 2, _xOffset);
  getstr(input);
  attroff(COLOR_PAIR(9));
  noecho();
  string name(input);
  _game->setName(name);
  return 0;
}