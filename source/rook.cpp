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


Rook::Rook(const bool player, const int x, const int y, Game* game, bool didntMove):
Piece(player, x, y, game)
{
  _repr = 'R';
  _didntMove = didntMove;
}


bool Rook::isRook() const noexcept
{
  return true;
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

  int size = _game->SIZE;
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
  _game->filterMoves(this, _savedMoves);
  return _savedMoves;
}


bool Rook::threat(Piece* piece)
{
  if (_x == piece->x())
  {
    int lowerBound = _y < piece->y() ? _y + 1 : piece->y() + 1,
        upperBound = _y < piece->y() ? piece->y() : _y;
    for (int index = lowerBound; index < upperBound; ++ index)
    {
      if (_game->at(_x, index) != nullptr)
      {
        return false;
      }
    }
    return true;
  }
  else if (_y == piece->y())
  {
    int lowerBound = _x < piece->x() ? _x + 1 : piece->x() + 1,
    upperBound = _x < piece->x() ? piece->x() : _x;
    for (int index = lowerBound; index < upperBound; ++ index)
    {
      if (_game->at(index, _y) != nullptr)
      {
        return false;
      }
    }
    return true;
  }
  return false;
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


bool Rook::didntMove() const noexcept
{
  return _didntMove;
}


void Rook::move(const int x, const int y) noexcept
{
  _didntMove = false;
  simulateMove(x, y);
}
