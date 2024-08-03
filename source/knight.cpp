/**
 * @file knight.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Knight class
 * @version 0.1
 * @date 01-08-2024
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
# include "../header/knight.hpp"
# include "../header/piece.hpp"


const vector<vector<int>> KNIGHT_MOVES = {{2, 1}, {-2, 1}, {2, -1}, {-2, -1}, {1, 2}, {-1, 2}, {1, -2}, {-1, -2}};


Knight::Knight():
Piece()
{
  _repr = 'N';
}


Knight::Knight(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'N';
}


bool Knight::isKnight() const noexcept
{
  return true;
}


void Knight::move(const int x, const int y)
{

}


vector<vector<int>> Knight::read() noexcept
{
  /*
    X X
   X   X
     N  
   X   X
    X X
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();

  int size = _game->SIZE, offsetX, offsetY;
  _savedMoves.reserve(KNIGHT_MOVES.size());
  for (const vector<int>& move : KNIGHT_MOVES)
  {
    offsetX = _x + move[0];
    offsetY = _y + move[1];
    Piece* target = _game->at(offsetX, offsetY);
    if (0 <= offsetX and 0 <= offsetY and offsetX < size and offsetY < size and  (target == nullptr or target->player() != this->player()))
    {
      _savedMoves.push_back({offsetX, offsetY});
    }
  }
  return _savedMoves;
}


ostream& operator<<(ostream& stream, const Knight& me)
{
  stream << "Knight(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}