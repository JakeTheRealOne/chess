/**
 * @file king.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the King class
 * @version 0.1
 * @date 02-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Std inclusions: ####
# include <iostream>
# include <vector>
using namespace std;

// #### Internal inclusions: ####
# include "../header/game.hpp"
# include "../header/king.hpp"
# include "../header/piece.hpp"
# include "../header/rook.hpp"


King::King():
Piece()
{
  _repr = 'K';
}


King::King(const bool player, const int x, const int y, Game* game, bool didntMove):
Piece(player, x, y, game)
{
  _repr = 'K';
  _didntMove = didntMove;
}


bool King::isKing() const noexcept
{
  return true;
}


vector<vector<int>> King::read() noexcept
{
  /*
   XXX
   XKX
   XXX
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();

  int size = _game->SIZE, offsetX, offsetY;
  _savedMoves.reserve(8);
  for (int i = -1; i < 2; ++ i)
  {
    for (int j = -1; j < 2; ++ j)
    {
      offsetX = _x + i;
      offsetY = _y + j;
      Piece* target = _game->at(offsetX, offsetY);
      if (offsetX >= 0 and offsetY >= 0 and offsetX < size and offsetY < size and (target == nullptr or target->player() != this->player()))
      {
        _savedMoves.push_back({offsetX, offsetY});
      }
    }
  }
  _game->filterMoves(this, _savedMoves);
  readCastling();
  return _savedMoves;
}


bool King::threat(Piece* piece)
{
  int diffX = abs(_x - piece->x()), diffY = abs(_y - piece->y());
  return diffX < 2 and diffY < 2 and (diffX or diffY);
}


ostream& operator<<(ostream& stream, const King& me)
{
  stream << "King(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


bool King::didntMove() const noexcept
{
  return _didntMove;
}


void King::move(const int x, const int y) noexcept
{
  _didntMove = false;
  simulateMove(x, y);
}


void King::readCastling() noexcept
{
  if (_game->checkList().size() or not _didntMove)
  {
    return; // Castling cannot be played when in check or if moved
  }

  Piece* rook;
  // long
  rook = _game->at(0, _y);
  if (rook != nullptr and rook->isRook() and ((Rook*)rook)->didntMove())
  {
    if (castlingLegal((Rook*)rook))
    {
      _savedMoves.push_back({_x - 2, _y});
    }
  }
  // short
  rook = _game->at(_game->SIZE - 1, _y);
  if (rook != nullptr and rook->isRook() and ((Rook*)rook)->didntMove())
  {
    if (castlingLegal((Rook*)rook))
    {
      _savedMoves.push_back({_x + 2, _y});
    }
  }
}


void King::executeCastling(bool isShort) noexcept
{

}

# include <ncurses.h>
bool King::castlingLegal(Rook* rook) const noexcept
{
  int increment = rook->x() < _x ? +1 : -1;
  int current = rook->x() + increment;
  // Space between king and rook has to be empty:
  while (current != _x)
  {
    if (_game->at(current, _y) != nullptr)
    {
      return false;
    }
    current += increment;
  }
  // The two adj squares has to be safe for the king
  increment *= -1;
  vector<vector<int>> kingPath = {{_x + increment, _y}, {_x + (increment << 1), _y}};
  _game->filterKingMoves((Piece*)this, kingPath);
  return kingPath.size() == 2;
}