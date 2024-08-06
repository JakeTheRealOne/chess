/**
 * @file game.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Game class
 * @version 0.1
 * @date 29-07-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Internal inclusions: ####
# include "../header/piece.hpp"
# include "../header/knight.hpp"
# include "../header/queen.hpp"
# include "../header/pawn.hpp"
# include "../header/bishop.hpp"
# include "../header/rook.hpp"
# include "../header/king.hpp"
# include "../header/game.hpp"

// #### Ncurses inclusion: ####
# include <ncurses.h>

// #### Std inclusions: ####
# include <vector>
# include <iostream>
# include <algorithm>
using namespace std;


Game::Game()
{
  for (int y = 0; y < SIZE; ++ y)
  {
    _board.push_back(vector<Piece*>(SIZE, nullptr));
    _drawingBox.push_back(vector<bool>(SIZE, false));
  }

  for (int x = 0; x < SIZE; ++ x) // Pawns
  {
    _board[1][x] = new Pawn(1, x, 1, this);
    _board[6][x] = new Pawn(0, x, 6, this);
  }

  _board[0][1] = new Knight(1, 1, 0, this);
  _board[0][6] = new Knight(1, 6, 0, this);
  _board[7][1] = new Knight(0, 1, 7, this);
  _board[7][6] = new Knight(0, 6, 7, this);

  _board[0][0] = new Rook(1, 0, 0, this);
  _board[0][7] = new Rook(1, 7, 0, this);
  _board[7][0] = new Rook(0, 0, 7, this);
  _board[7][7] = new Rook(0, 7, 7, this);

  _board[0][2] = new Bishop(1, 2, 0, this);
  _board[0][5] = new Bishop(1, 5, 0, this);
  _board[7][2] = new Bishop(0, 2, 7, this);
  _board[7][5] = new Bishop(0, 5, 7, this);

  _board[0][3] = new Queen(1, 3, 0, this);
  _board[7][3] = new Queen(0, 3, 7, this);

  _whiteKing = new King(0, 4, 7, this);
  _blackKing = new King(1, 4, 0, this);
  _board[7][4] = _whiteKing;
  _board[0][4] = _blackKing;

  // debug:
  _board[4][3] = new Pawn(1, 3, 4, this);
}


Game::Game(string path)
{
  // COMING SOON
}

Game::~Game()
{
  int count = 0; //< for debug
  for (const vector<Piece*>& row : _board)
  {
    for (const Piece* piece : row)
    {
      if (piece != nullptr)
      {
        delete piece;
        ++ count;
      }
    }
  }
  cout << "[DBG] " << count << " piece.s deleted" << endl;
}


vector<vector<Piece*>> Game::board() const noexcept
{
  return this->_board;
}


bool Game::turn() const noexcept
{
  return this->_turn;
}


Piece* Game::at(const int x, const int y) const noexcept
{
  if (x < 0 or y < 0 or x > SIZE or y > SIZE)
  {
    return nullptr;
  }
  return _board[y][x];
}


vector<Piece*> Game::checkList() const noexcept
{
  return this->_checkList;
}


void Game::display(const int currentX, const int currentY) const noexcept
{
  Piece* piece;
  for (int i = 0; i < SIZE; ++ i)
  {
    for (int j = 0; j < SIZE; ++ j)
    {
      piece = _board[i][j];
      (i == currentY and j == currentX) ? attron(COLOR_PAIR(4)) : 0;
      printw("%c ", (piece == nullptr ? '.' : piece->repr()));
      attroff(COLOR_PAIR(4));
    }
    printw("\n");
  }
}


Piece* Game::king(const bool player) const noexcept
{
  return player ? _blackKing : _whiteKing;
}


int Game::index() const noexcept
{
  return this->_index;
}


void Game::showMoves(const int x, const int y, const int currentX, const int currentY) noexcept
{
  Piece* piece = at(x, y);
  if (piece == nullptr)
  {
    display();
    return;
  }
  const vector<vector<int>> moves = piece->read();
  for (const vector<int>& move : moves)
  {
    _drawingBox[move[1]][move[0]] = true;
  }
  for (int i = 0; i < SIZE; ++ i)
  {
    for (int j = 0; j < SIZE; ++ j)
    {
      (i == currentY and j == currentX) ? attron(COLOR_PAIR(4)) : 0;
      printw("%c ", _drawingBox[i][j] ? 'X' : _board[i][j] == nullptr ? '.' : _board[i][j]->repr());
      attroff(COLOR_PAIR(4));
    }
    printw("\n");
  }
  for (const vector<int>& move : moves)
  {
    _drawingBox[move[1]][move[0]] = false;
  }
}


bool Game::move(Piece* piece, const int x, const int y) noexcept
{
  // Check the legality of the move:
  vector<int> pos = {x, y};
  vector<vector<int>> available = (piece == nullptr ? vector<vector<int>>(/*empty list*/) : piece->read());
  if (find(available.begin(), available.end(), pos) == available.end())
  {
    return 1;
  }
  if (_board[y][x] != nullptr)
  {
    delete _board[y][x];
  }
  _board[y][x] = piece;
  _board[piece->y()][piece->x()] = nullptr;
  piece->move(x, y);
  // Update checkList:
  return 0;
}


void Game::filterMoves(Piece* piece, vector<vector<int>>& moves) const noexcept
{

}

bool Game::operator==(const Game& other) const
{
  return (other._turn == this->_turn and other._board == this->_board);
}


Game& Game::operator++()
{
  _turn = not _turn;
  ++ _index;
  return *this;
}
