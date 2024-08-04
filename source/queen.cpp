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