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


void Queen::move(const int x, const int y)
{

}


vector<vector<int>> Queen::read() const noexcept
{
  /*
    X X X
     XXX
    XXQXX
     XXX
    x x x
  */
  int size = _game->SIZE, offsetX, offsetY;
  vector<vector<int>> moves;
  //this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const Queen& me)
{
  stream << "Queen(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void Queen::filterMoves(vector<vector<int>>& moves) const noexcept
{
}