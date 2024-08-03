/**
 * @file piece.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Piece class
 * @version 0.1
 * @date 01-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Internal inclusions: ####
# include "../header/piece.hpp"
# include "../header/game.hpp"

// #### Std inclusions: ####
# include <stdexcept>
# include <iostream>
# include <vector>
using namespace std;


Piece::Piece(const bool player, const int x, const int y, Game* game):
_player(player), _x(x), _y(y), _game(game)
{}


bool Piece::player() const noexcept
{
  return this->_player;
}


int Piece::x() const noexcept
{
  return this->_x;
}


int Piece::y() const noexcept
{
  return this->_y;
}


char Piece::repr() const noexcept
{
  return this->_repr;
}

Game* Piece::game() const noexcept
{
  return this->_game;
}


bool Piece::isKing() const noexcept
{
  return false;
}


bool Piece::isQueen() const noexcept
{
  return false;
}


bool Piece::isRook() const noexcept
{
  return false;
}


bool Piece::isKnight() const noexcept
{
  return false;
}


bool Piece::isBishop() const noexcept
{
  return false;
}


bool Piece::isPawn() const noexcept
{
  return false;
}


bool Piece::isPinned() const noexcept
{
  // check if diag or col or row of the piece is the same as their king and check if Q B R on it too
  Piece* myKing = _game->king(_player);
  if (myKing->x() == _x) // Column
  {
    return checkCol(myKing);
  }
  else if (myKing->y() == _y) // Row
  {
    return checkRow(myKing);
  }
  else if (false) // Diagonal 1
  {

  }
  else if (false) // Diagonal 2
  {

  }
  return false;
}


bool Piece::checkCol(Piece* king) const noexcept
{
  // int start, end;
  // if (king->y() < _y)
  // {
  //   start = king->y();
  //   end = _y;
  // }
  // else
  // {
  //   start = _y;
  //   end = king->y();
  // }
  // // check the space between
  // for (int i = start; i < end; ++ i)
  // {
  //   if (_game->at(_x, i) != nullptr)
  //   {
  //     return false;
  //   }
  // }
  // check the space after for any Q or R
  return false;
}


bool Piece::checkRow(Piece* king) const noexcept
{
  return false;
}


void Piece::move(const int x, const int y)
{
  throw runtime_error(
    "Piece::move should only be called on a non-empty piece (inheritance)"
  );
}


vector<vector<int>> Piece::read() const
{
  throw runtime_error(
    "Piece::read should only be called on a non-empty piece (inheritance)"
  );
}


bool Piece::operator==(const Piece& other) const noexcept
{
  return (this->_player == other._player)
     and (this->_x == other._x)
     and (this->_y == other._y);
}


ostream& operator<<(ostream& stream, const Piece& me)
{
  stream << "Piece(" << (me._player ? "black" : "white") << ", " << me._x << ", " << me._y << ")";
  return stream;
}