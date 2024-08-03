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


void Bishop::move(const int x, const int y)
{

}


vector<vector<int>> Bishop::read() const noexcept
{
  /*
    X   X
     X X
      B
     X X
    X   X
  */
  int size = _game->SIZE, offsetX, offsetY;
  vector<vector<int>> moves;
  //this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const Bishop& me)
{
  stream << "Bishop(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void Bishop::filterMoves(vector<vector<int>>& moves) const noexcept
{
}