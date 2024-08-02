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
# include "../header/pawn.hpp"
# include "../header/king.hpp"
# include "../header/game.hpp"

// #### Std inclusions: ####
# include <vector>
# include <iostream>
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
  whiteKing = new King(0, 4, 7, this);
  blackKing = new King(1, 4, 0, this);
  _board[7][4] = whiteKing;
  _board[0][4] = blackKing;

  // _board[2][3] = new Knight(0, 3, 2, this);
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
    cout << "[WRN] " << (_turn ? "Black" : "White") << " try to get a piece outside the box" << endl;
    return nullptr;
  }
  return _board[y][x];
}


vector<Piece*> Game::checkList() const noexcept
{
  return this->_checkList;
}

void Game::display() const noexcept
{
  for (const vector<Piece*>& row : _board)
  {
    for (const Piece* piece : row)
    {
      cout << (piece == nullptr ? '.' : piece->repr()) << " ";
    }
    cout << endl;
  }
}


void Game::showMoves(const int x, const int y) noexcept
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
      cout << (_drawingBox[i][j] ? 'X' : _board[i][j] == nullptr ? '.' : _board[i][j]->repr()) << " ";
    }
    cout << endl;
  }
  for (const vector<int>& move : moves)
  {
    _drawingBox[move[1]][move[0]] = false;
  }
}


bool Game::operator==(const Game& other) const
{
  return (other._turn == this->_turn and other._board == this->_board);
}