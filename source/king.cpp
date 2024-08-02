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


King::King(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'K';
}


bool King::isKing() const noexcept
{
  return true;
}


void King::move(const int x, const int y)
{

}


vector<vector<int>> King::read() const noexcept
{
  /*
   XXX
   XKX
   XXX
  */
  int size = _game->SIZE, offsetX, offsetY;
  vector<vector<int>> moves;
  moves.reserve(8);
  for (int i = -1; i < 2; ++ i)
  {
    for (int j = -1; j < 2; ++ j)
    {
      offsetX = _x + i;
      offsetY = _y + j;
      Piece* target = _game->at(offsetX, offsetY);
      if (offsetX >= 0 and offsetY >= 0 and offsetX < size and offsetY < size and (target == nullptr or target->player() != this->player()))
      {
        moves.push_back({offsetX, offsetY});
      }
    }
  }
  this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const King& me)
{
  stream << "King(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void King::filterMoves(vector<vector<int>>& moves) const noexcept
{
  /*
  LEGAL MOVES:
  - Do put itself in check
  */
  
}