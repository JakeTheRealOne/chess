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