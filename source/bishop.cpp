/**
 * @file bishop.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Bishop class
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
# include "../header/bishop.hpp"
# include "../header/piece.hpp"


Bishop::Bishop():
Piece()
{
  _repr = 'B';
}


Bishop::Bishop(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'B';
}


bool Bishop::isBishop() const noexcept
{
  return true;
}


vector<vector<int>> Bishop::read() noexcept
{
  /*
    X   X
     X X
      B
     X X
    X   X
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();

  int size = _game->SIZE, i, j, increI, increJ;
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
  _game->filterMoves(this, _savedMoves);
  return _savedMoves;
}


bool Bishop::threat(Piece* piece){
  if (_x - _y == piece->x() - piece->y())
  { // Diag A
    int increment = _x < piece->x() ? +1 : -1;
    int currentX = _x + increment, currentY = _y + increment;
    while (currentX != piece->x())
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
    while (currentX != piece->x())
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


ostream& operator<<(ostream& stream, const Bishop& me)
{
  stream << "Bishop(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


bool Bishop::checkTarget(const int x, const int y) noexcept
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