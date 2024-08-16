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


Pawn::Pawn(const bool player, const int x, const int y, Game* game, int doubleUp):
Piece(player, x, y, game)
{
  _doubleUp = doubleUp;
  _repr = 'P';
}


bool Pawn::isPawn() const noexcept
{
  return true;
}


bool Pawn::didntMove() const noexcept
{
  return _doubleUp == -1;
}

int Pawn::doubleUpIndex() const noexcept
{
  return _doubleUp;
}

void Pawn::move(const int x, const int y) noexcept
{
  if (abs(_y - y) == 2)
  {
    _doubleUp = _game->index(); //< Register double up index for En passant
  }
  else if (didntMove())
  {
    _doubleUp = -2;
  }
  _x = x;
  _y = y;
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
    if (didntMove() and offsetY >= 0 and offsetY < size and _game->at(_x, offsetY) == nullptr)
    {
      _savedMoves.push_back({_x, offsetY});   
    }
  }
  offsetY = _y + (_player ? 1 : -1);
  if (_game->at(_x + 1, offsetY) != nullptr and _game->at(_x + 1, offsetY)->player() != _player)
  {
    _savedMoves.push_back({_x + 1, offsetY});   
  }
  else if (((_player and _y == 4) or (not _player and _y == 3)) and _game->at(_x + 1, _y) != nullptr and _game->at(_x + 1, _y)->isPawn())
  { // Possible en passant;
    Pawn* pawn = (Pawn*)_game->at(_x + 1, _y);
    if (pawn->player() != _player and pawn->doubleUpIndex() == _game->index() - 1)
    {
      _savedMoves.push_back({_x + 1, offsetY});
    }
  }
  if (_game->at(_x - 1, offsetY) != nullptr and _game->at(_x - 1, offsetY)->player() != _player)
  {
    _savedMoves.push_back({_x - 1, offsetY});   
  }
  else if (((_player and _y == 4) or (not _player and _y == 3)) and _game->at(_x - 1, _y) != nullptr and _game->at(_x - 1, _y)->isPawn())
  { // Possible en passant;
    Pawn* pawn = (Pawn*)_game->at(_x - 1, _y);
    if (pawn->player() != _player and pawn->doubleUpIndex() == _game->index() - 1)
    {
    _savedMoves.push_back({_x - 1, offsetY});
    }
  }
  _game->filterMoves(this, _savedMoves);
  return _savedMoves;
}


bool Pawn::threat(Piece* piece)
{
  int diffX = abs(_x - piece->x()), diffY = _y - piece->y();
  return diffX == 1 and (_player ? -1 : +1) == diffY;
}

ostream& operator<<(ostream& stream, const Pawn& me)
{
  stream << "Pawn(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}
