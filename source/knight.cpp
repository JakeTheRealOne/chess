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


vector<vector<int>> Knight::read() const noexcept
{
  /*
    X X
   X   X
     N  
   X   X
    X X
  */
  int size = _game->SIZE, offsetX, offsetY;
  vector<vector<int>> moves;
  moves.reserve(KNIGHT_MOVES.size());
  for (const vector<int>& move : KNIGHT_MOVES)
  {
    offsetX = _x + move[0];
    offsetY = _y + move[1];
    Piece* target = _game->at(offsetX, offsetY);
    if (0 <= offsetX and 0 <= offsetY and offsetX < size and offsetY < size and  (target == nullptr or target->player() != this->player()))
    {
      moves.push_back({offsetX, offsetY});
    }
  }
  //this->filterMoves(moves);
  return moves;
}


ostream& operator<<(ostream& stream, const Knight& me)
{
  stream << "Knight(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}


void Knight::filterMoves(vector<vector<int>>& moves) const noexcept
{
  /*
  LEGAL MOVES:
  - Do not create a check
  - Block a current check
  - Do not eat a friend piece
  */
  vector<Piece*> currentChecks = _game->checkList();
  if (currentChecks.size() > 1 or isPinned())
  {
    moves.clear(); //< There is nothing a knight can do
  }
  /// If check is N or P, the knight has to eat it
  /// If check is Q, B, R, the knight has to stand between (or eat but ==)
  /// If no check, the knight is free to go anywhere if
    //- Not eating firend piece
    //- Not pinned
  else if (currentChecks.size())
  {
    Piece* piece = currentChecks[0];
    if (piece->isKnight() or piece->isPawn())
    {
      /// Find a move that is eating the ennemy
      for (auto iter = moves.begin(); iter != moves.end(); ++ iter)
      {
        if (_game->at((*iter)[0], (*iter)[1]) == piece)
        {
          cout << "Can be eaten" << endl;
          // !!! Check if pinned here
          moves[0] = *iter;
          moves.erase(moves.begin() + 1, moves.end());
          return;
        }
      }
      moves.clear();
    }
    else
    {
      // piece is Q, B, R
    }
  }
  else
  {

  }
}