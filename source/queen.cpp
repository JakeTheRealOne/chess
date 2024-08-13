/**
 * @file queen.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Queen class
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
# include "../header/queen.hpp"
# include "../header/piece.hpp"


Queen::Queen():
Piece()
{
  _repr = 'Q';
}


Queen::Queen(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'Q';
}


bool Queen::isQueen() const noexcept
{
  return true;
}


vector<vector<int>> Queen::read() noexcept
{
  /*
    X X X
     XXX
    XXQXX
     XXX
    x x x
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();

  int size = _game->SIZE, offsetX, offsetY, i, j, increI, increJ;
  // Bishop behavior:
  for (int a = 0; a < 4; ++ a)
  {
    increI = a >> 1 ? 1 : -1;
    increJ = a & 1 ? 1 : -1;
    i = _x + increI;
    j = _y + increJ;
    while (i >= 0 and i < size and j >= 0 and j < size)
    {
      if (not checkTarget(i, j))
      {
        break;
      }
      i += increI;
      j += increJ;
    }
  }
  // Rook behavior:
  for (int i = _x + 1; i < size; ++ i)
  {
    if (not checkTarget(i, _y))
    {
      break;
    }
  }
  for (int i = _x - 1; i >= 0; -- i)
  {
    if (not checkTarget(i, _y))
    {
      break;
    }
  }
  for (int i = _y + 1; i < size; ++ i)
  {
    if (not checkTarget(_x, i))
    {
      break;
    }
  }
  for (int i = _y - 1; i >= 0; -- i)
  {
    if (not checkTarget(_x, i))
    {
      break;
    }
  }
  _game->filterMoves(this, _savedMoves);
  return _savedMoves;
}


bool Queen::threat(Piece* piece)
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
  else if (_x - _y == piece->x() - piece->y())
  { // Diag A
    int increment = _x < piece->x() ? +1 : -1;
    int currentX = _x + increment, currentY = _y + increment;
    while (currentX < piece->x())
    {
      if (_game->at(currentX, currentY) != nullptr)
      {
        return false;
      }
      currentX += increment, currentY += increment;
    }
    return true;
  }
  else if (_x + _y == piece->x() + piece->y())
  { // Diag B
    int increment = _x < piece->x() ? +1 : -1;
    int currentX = _x + increment, currentY = _y - increment;
    while (currentX < piece->x())
    {
      if (_game->at(currentX, currentY) != nullptr)
      {
        return false;
      }
      currentX += increment, currentY -= increment;
    }
    return true;
  }
  return false;
}


ostream& operator<<(ostream& stream, const Queen& me)
{
  stream << "Queen(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


bool Queen::checkTarget(const int x, const int y) noexcept
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