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


vector<vector<int>> Rook::read() noexcept
{
  /*
      X
      X
    XXRXX
      X
      X
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();

  int size = _game->SIZE, offsetX, offsetY;
  for (int i = _x + 1; i < size; ++ i)
  {
    if (not checkTarget(i, _y)){
      break;
    }
  }
  for (int i = _x - 1; i >= 0; -- i)
  {
    if (not checkTarget(i, _y)){
      break;
    }
  }
  for (int i = _y + 1; i < size; ++ i)
  {
    if (not checkTarget(_x, i)){
      break;
    }
  }
  for (int i = _y - 1; i >= 0; -- i)
  {
    if (not checkTarget(_x, i)){
      break;
    }
  }
  return _savedMoves;
}


ostream& operator<<(ostream& stream, const Rook& me)
{
  stream << "Rook(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


bool Rook::checkTarget(const int x, const int y) noexcept
{
    Piece* target = _game->at(x, y);
    if (target == nullptr)
    {
      _savedMoves.push_back({x, y});
      return true;
    }
    else if (target->player() != _player)
    {
      _savedMoves.push_back({x, y});
    }
    return false;
}