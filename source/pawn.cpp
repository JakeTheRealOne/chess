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


vector<vector<int>> Pawn::read() noexcept
{
  /*
     X 
    XXX
     P
    (take and en passant conditions for diag. + first move)
  */
  if (_game->index() == _savedIndex)
  {
    return _savedMoves;
  }
  _savedMoves.clear();
  _savedIndex = _game->index();


  int size = _game->SIZE;
  int offsetY = _y + (_player ? 1 : -1);
  auto tmp = _game->at(_x, offsetY);
  if (offsetY >= 0 and offsetY < size and _game->at(_x, offsetY) == nullptr)
  {
    _savedMoves.push_back({_x, offsetY});
    _player ? ++ offsetY : -- offsetY;
    if (_didntMove and offsetY >= 0 and offsetY < size and _game->at(_x, offsetY) == nullptr)
    {
      _savedMoves.push_back({_x, offsetY});   
    }
  }
  offsetY = _y + (_player ? 1 : -1);
  if (_game->at(_x + 1, offsetY) != nullptr)
  {
    _savedMoves.push_back({_x + 1, offsetY});   
  }
  if (_game->at(_x - 1, offsetY) != nullptr)
  {
    _savedMoves.push_back({_x - 1, offsetY});   
  }
  return _savedMoves;
}


ostream& operator<<(ostream& stream, const Pawn& me)
{
  stream << "Pawn(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}
