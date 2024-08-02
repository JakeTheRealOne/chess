/**
 * @file pawn.cpp
 * @author JakeTheRealOne (jakelevrai@outook.be)
 * @brief Source file for the Pawn class
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
# include "../header/pawn.hpp"
# include "../header/piece.hpp"


Pawn::Pawn():
Piece()
{
  _repr = 'P';
}


Pawn::Pawn(const bool player, const int x, const int y, Game* game):
Piece(player, x, y, game)
{
  _repr = 'P';
}


bool Pawn::isPawn() const noexcept
{
  return true;
}


bool Pawn::didntMove() const noexcept
{
  return _didntMove;
}

void Pawn::move(const int x, const int y)
{
  _didntMove = false;
}


vector<vector<int>> Pawn::read() const noexcept
{
  /*
     X 
    XXX
     P
    (take and en passant conditions for diag. + first move)
  */
  int size = _game->SIZE;
  vector<vector<int>> moves;
  int offsetY = _y + (_player ? 1 : -1);
  auto tmp = _game->at(_x, offsetY);
  if (offsetY >= 0 and offsetY < size and _game->at(_x, offsetY) == nullptr)
  {
    moves.push_back({_x, offsetY});
    cout << offsetY << endl;
    _player ? ++ offsetY : -- offsetY;
    cout << offsetY << endl;
    if (_didntMove and offsetY >= 0 and offsetY < size and _game->at(_x, offsetY) == nullptr)
    {
      moves.push_back({_x, offsetY});   
    }
  }
  offsetY = _y + (_player ? 1 : -1);
  if (_game->at(_x + 1, offsetY) != nullptr)
  {
    moves.push_back({_x + 1, offsetY});   
  }
  if (_game->at(_x - 1, offsetY) != nullptr)
  {
    moves.push_back({_x - 1, offsetY});   
  }
  //this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const Pawn& me)
{
  stream << "Pawn(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void Pawn::filterMoves(vector<vector<int>>& moves) const noexcept
{

}