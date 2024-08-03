/**
 * @file rook.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Rook class
 * @version 0.1
 * @date 03-08-2024
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
# include "../header/rook.hpp"
# include "../header/piece.hpp"


Rook::Rook():
Piece()
{
  _repr = 'R';
}


Rook::Rook(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'R';
}


bool Rook::isRook() const noexcept
{
  return true;
}


void Rook::move(const int x, const int y)
{

}


vector<vector<int>> Rook::read() const noexcept
{
  /*
      X
      X
    XXRXX
      X
      X
  */
  int size = _game->SIZE, offsetX, offsetY;
  vector<vector<int>> moves;
  for (int i = _x + 1; i < size; ++ i)
  {
    if (not tmp(moves, i, _y)){
      break;
    }
  }
  for (int i = _x - 1; i >= 0; -- i)
  {
    if (not tmp(moves, i, _y)){
      break;
    }
  }
  for (int i = _y + 1; i < size; ++ i)
  {
    if (not tmp(moves, _x, i)){
      break;
    }
  }
  for (int i = _y - 1; i >= 0; -- i)
  {
    if (not tmp(moves, _x, i)){
      break;
    }
  }
  //this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const Rook& me)
{
  stream << "Rook(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void Rook::filterMoves(vector<vector<int>>& moves) const noexcept
{
}


bool Rook::checkTarget(vector<vector<int>>& moves, const int x, const int y) const noexcept
{
    Piece* target = _game->at(x, y);
    if (target == nullptr)
    {
      moves.push_back({x, y});
      return true;
    }
    else if (target->player() != _player)
    {
      moves.push_back({x, y});
    }
    return false;
}