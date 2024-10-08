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


void Piece::simulateMove(const int x, const int y) noexcept
{
  _x = x;
  _y = y;
}


void Piece::move(const int x, const int y) noexcept
{
  simulateMove(x, y);
}


vector<vector<int>> Piece::read()
{
  throw runtime_error(
    "Piece::read should only be called on a non-empty piece (inheritance)"
  );
}


bool Piece::threat(Piece*)
{
  return false;
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