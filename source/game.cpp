/**
 * @file game.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Game class
 * @version 0.1
 * @date 29-07-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Internal inclusions: ####
# include "../header/piece.hpp"
# include "../header/knight.hpp"
# include "../header/queen.hpp"
# include "../header/pawn.hpp"
# include "../header/bishop.hpp"
# include "../header/rook.hpp"
# include "../header/king.hpp"
# include "../header/game.hpp"

// #### Ncurses inclusion: ####
# include <ncurses.h>

// #### Std inclusions: ####
# include <stdexcept>
# include <vector>
# include <iostream>
# include <algorithm>
using namespace std;


extern vector<vector<int>> KNIGHT_MOVES;

Game::Game()
{
  for (int y = 0; y < SIZE; ++ y)
  {
    _board.push_back(vector<Piece*>(SIZE, nullptr));
  }

  for (int x = 0; x < SIZE; ++ x) // Pawns
  {
    _board[1][x] = new Pawn(1, x, 1, this);
    _board[6][x] = new Pawn(0, x, 6, this);
  }

  _board[0][1] = new Knight(1, 1, 0, this);
  _board[0][6] = new Knight(1, 6, 0, this);
  _board[7][1] = new Knight(0, 1, 7, this);
  _board[7][6] = new Knight(0, 6, 7, this);

  _board[0][0] = new Rook(1, 0, 0, this);
  _board[0][7] = new Rook(1, 7, 0, this);
  _board[7][0] = new Rook(0, 0, 7, this);
  _board[7][7] = new Rook(0, 7, 7, this);

  _board[0][2] = new Bishop(1, 2, 0, this);
  _board[0][5] = new Bishop(1, 5, 0, this);
  _board[7][2] = new Bishop(0, 2, 7, this);
  _board[7][5] = new Bishop(0, 5, 7, this);

  _board[0][3] = new Queen(1, 3, 0, this);
  _board[7][3] = new Queen(0, 3, 7, this);

  // _whiteKing = new King(0, 4, 7, this);
  _blackKing = new King(1, 4, 0, this);
  // _board[7][4] = _whiteKing;
  _board[0][4] = _blackKing;

  // debug:
  _whiteKing = new King(0, 5, 4, this);
  _board[4][5] = _whiteKing;
  _board[4][3] = new Knight(0, 3, 4, this);
  _board[4][1] = new Rook(1, 1, 4, this);

}


Game::Game(string path)
{
  // COMING SOON
}


Game::~Game()
{
  int count = 0; //< for debug
  for (const vector<Piece*>& row : _board)
  {
    for (const Piece* piece : row)
    {
      if (piece != nullptr)
      {
        delete piece;
        ++ count;
      }
    }
  }
  cout << "[DBG] " << count << " piece.s deleted" << endl;
}


vector<vector<Piece*>> Game::board() const noexcept
{
  return this->_board;
}


bool Game::turn() const noexcept
{
  return this->_turn;
}


Piece* Game::at(const int x, const int y) const noexcept
{
  if (x < 0 or y < 0 or x >= SIZE or y >= SIZE)
  {
    return nullptr;
  }
  return _board[y][x];
}


vector<Piece*> Game::checkList() const noexcept
{
  return this->_checkList;
}


Piece* Game::king(const bool player) const noexcept
{
  return player ? _blackKing : _whiteKing;
}


int Game::index() const noexcept
{
  return this->_index;
}


bool Game::move(Piece* piece, const int x, const int y, const bool force) noexcept
{
  // Check the legality of the move:
  vector<int> pos = {x, y};
  vector<vector<int>> available = (piece == nullptr ? vector<vector<int>>(/*empty list*/) : piece->read());
  if (not force and find(available.begin(), available.end(), pos) == available.end())
  {
    return 1;
  }
  if (_board[y][x] != nullptr)
  {
    delete _board[y][x];
  }
  else if (piece->isPawn() and piece->x() != x)
  {
    delete _board[piece->y()][x];
    _board[piece->y()][x] = nullptr;
  }
  _board[y][x] = piece;
  _board[piece->y()][piece->x()] = nullptr;
  piece->move(x, y);

  static int tmp = 1;
  ++ tmp;
  return 0;
}


void Game::filterMoves(Piece* piece, vector<vector<int>>& moves)
{
  /*
  If K:
    -> Accept all the moves that resolve current checks in checkList
    -> & That does not put the K in danger
  Else:
    -> Refuse all moves if checkList.size() > 1
    -> Accept all the moves that eat the possible current check
    -> | block the possible current check (if check is Q B R)
    -> & does not discover a check
  */
  if (piece->isKing())
  {
    filterKingMoves(piece, moves);
  }
  else
  {
    filterNotKingMoves(piece, moves);
  }
}


void Game::filterKingMoves(Piece* piece, vector<vector<int>>& moves)
{
  int x = piece->x(), y = piece->y(), moveX, moveY, n = moves.size();
  Piece* previous;
  for (int i = 0; i < n; ++ i)
  {
    moveX = moves[i][0], moveY = moves[i][1];
    previous = _board[moveY][moveX];
    _board[moveY][moveX] = piece;
    _board[y][x] = previous;
    piece->move(moveX, moveY);
    bool notSafe = knightNear(piece) or rookAndQueenNear(piece) or bishopAndQueenNear(piece) or pawnNear(piece) or kingNear(piece);
    _board[moveY][moveX] = previous;
    _board[y][x] = piece;
    piece->move(x, y);
    if (notSafe)
    {
      swap(moves[i], moves.back());
      moves.pop_back();
      -- n;
      -- i;
    }
  }
}



void Game::filterNotKingMoves(Piece* piece, vector<vector<int>>& moves)
{
  if (_checkList.size() > 1) // double check cannot be stopped by non-king pieces
  {
    moves.clear();
    return;
  }
  Piece* myKing = king(piece->player());
  int x = piece->x(), y = piece->y(), moveX, moveY, n = moves.size();
  if (_checkList.size()) // single check: has to be stopped
  {
    Piece* threat = _checkList[0];
    bool canBeBlocked = threat->isRook() or threat->isBishop() or threat->isQueen();
    for (int i = 0; i < n; ++ i)
    {
      moveX = moves[i][0], moveY = moves[i][1];
      if (moveX == threat->x() and moveY == threat->y())
      {
        continue;
      }
      if (canBeBlocked and _board[moveY][moveX] == nullptr)
      {
        _board[moveY][moveX] = piece;
        _board[y][x] = nullptr;
        piece->move(moveX, moveY);
        bool blocked = not threat->threat(myKing);
        _board[moveY][moveX] = nullptr;
        _board[y][x] = piece;
        piece->move(x, y);
        if (blocked)
        {
          continue;
        }
      }
      swap(moves[i], moves.back());
      moves.pop_back();
      -- n;
      -- i;
    }
  }
  // Check if the piece is pinned & remove moves that leave the restricted area of pin (I know what Im talking about)
  Piece* pinned = isDiscoveryCheck(piece->x(), piece->y(), piece->player());
  if (pinned != nullptr)
  {
    for (int i = 0; i < n; ++ i)
    {
      moveX = moves[i][0], moveY = moves[i][1];
      if (moveX == pinned->x() and moveY == pinned->y())
      {
        continue;
      }
      if (_board[moveY][moveX] == nullptr)
      {
        _board[moveY][moveX] = piece;
        _board[y][x] = nullptr;
        piece->move(moveX, moveY);
        bool blocked = not pinned->threat(myKing);
        _board[moveY][moveX] = nullptr;
        _board[y][x] = piece;
        piece->move(x, y);
        if (blocked)
        {
          continue;
        }
      }
      swap(moves[i], moves.back());
      moves.pop_back();
      -- n;
      -- i;   
    } 
  }
}


void Game::updateCheckList(Piece* piece, const int x, const int y) noexcept
{
  _checkList.clear();
  // Direct check
  if (piece->threat(king(not piece->player())))
  {
    // mvprintw(2, 0, "DIRECT THREAT %s", string(1, piece->repr()).c_str()); // DEBUG
    _checkList.push_back(piece);
  }
  // Discovery check
  if (piece->isKing())
  {
    return; //< Do not check discovery checks because the king cannot be pinned to itself
  }
  else if (piece->isPawn() and (piece->player() ? 4 : 3) == y and x != piece->x() and _board[y][piece->x()] == nullptr) // En passant
  {
    Piece* threat = isDiscoveryCheck(piece->x(), y, not piece->player());
    if (threat != nullptr)
    {
      _checkList.push_back(threat);
      return; // En passant cannot discover 2 different checks
    }
  }
  Piece* threat = isDiscoveryCheck(x, y,  not piece->player());
  if (threat != nullptr)
  {
    // mvprintw(3, 0, "DISCOVERY THREAT %s", string(1, threat->repr()).c_str()); // DEBUG
    _checkList.push_back(threat);
  }
}


Piece* Game::discoverRow(const int x, const int y, const bool player, Piece* king) const noexcept
{
  // Space between (x, y) and king as to be empty
  int lowerBound = x < king->x() ? x + 1 : king->x() + 1,
      upperBound = y < king->x() ? king->x() : x;
  for (int index = lowerBound; index < upperBound; ++ index)
  {
    if (_board[y][index] != nullptr)
    {
      return nullptr;
    }
  }
  // Return if there is directly a ROOK/QUEEN after (x, y)
  int increment = x < king->x() ? -1 : +1, index = x + increment;
  while (0 <= index and index < SIZE)
  {
    if (_board[y][index] == nullptr)
    {
      index += increment;
      continue;
    }
    else
    {
      return (
        _board[y][index]->player() != player and (_board[y][index]->isRook() or _board[y][index]->isQueen())
      ) ? _board[y][index] : nullptr;
    }
  }
  return nullptr;
}


Piece* Game::discoverCol(const int x, const int y, const bool player, Piece* king) const noexcept
{
  // Space between (x, y) and king as to be empty
  int lowerBound = y < king->y() ? y + 1 : king->y() + 1,
      upperBound = y < king->y() ? king->y() : y;
  for (int index = lowerBound; index < upperBound; ++ index)
  {
    if (_board[index][x] != nullptr)
    {
      return nullptr;
    }
  }
  // Return if there is directly a ROOK/QUEEN after (x, y)
  int increment = y < king->y() ? -1 : +1, index = y + increment;
  while (0 <= index and index < SIZE)
  {
    if (_board[index][x] == nullptr)
    {
      index += increment;
      continue;
    }
    else
    {
      return (
        _board[index][x]->player() != player and (_board[index][x]->isRook() or _board[index][x]->isQueen())
      ) ? _board[index][x] : nullptr;
    }
  }
  return nullptr;
}


Piece* Game::discoverDiagA(const int x, const int y, const bool player, Piece* king) const noexcept
{
  // Space between (x, y) and king as to be empty
  int increment = x < king->x() ? +1 : -1;
  int currentX = x + increment, currentY = y + increment;
  while (currentX < king->x())
  {
    if (_board[currentY][currentX] != nullptr)
    {
      return nullptr;
    }
    currentX += increment, currentY += increment;
  }
  // Return if there is directly a BISHOP/QUEEN after (x, y)
  increment = increment == 1 ? -1 : +1;
  currentX = x + increment, currentY = y + increment;
  while (0 <= currentX and currentX < SIZE and 0 <= currentY and currentY < SIZE)
  {
    if (_board[currentY][currentX] == nullptr)
    {
      currentX += increment, currentY += increment;
      continue;
    }
    else
    {
      return (
        _board[currentY][currentX]->player() != player and (_board[currentY][currentX]->isBishop() or _board[currentY][currentX]->isQueen())
      ) ? _board[currentY][currentX] : nullptr;
    }
  }
  return nullptr;
}


Piece* Game::discoverDiagB(const int x, const int y, const bool player, Piece* king) const noexcept
{
  // Space between (x, y) and king as to be empty
  int increment = x < king->x() ? +1 : -1;
  int currentX = x + increment, currentY = y - increment;
  while (currentX < king->x())
  {
    if (_board[currentY][currentX] != nullptr)
    {
      return nullptr;
    }
    currentX += increment, currentY -= increment;
  }
  // Return if there is directly a BISHOP/QUEEN after (x, y)
  increment = increment == 1 ? -1 : +1;
  currentX = x + increment, currentY = y - increment;
  while (0 <= currentX and currentX < SIZE and 0 <= currentY and currentY < SIZE)
  {
    if (_board[currentY][currentX] == nullptr)
    {
      currentX += increment, currentY -= increment;
      continue;
    }
    else
    {
      return (
        _board[currentY][currentX]->player() != player and (_board[currentY][currentX]->isBishop() or _board[currentY][currentX]->isQueen())
      ) ? _board[currentY][currentX] : nullptr;
    }
  }
  return nullptr;
}


Piece* Game::isDiscoveryCheck(const int x, const int y, const bool player) const noexcept
{
  Piece* king = this->king(player);
  if (king->x() == x) // Same col
  {
    return discoverCol(x, y, player, king);
  }
  else if (king->y() == y) // Same row
  {
    return discoverRow(x, y, player, king);
  }
  else if (x - y == king->x() - king->y()) // Same diag (Diag A)
  {
    return discoverDiagA(x, y, player, king);
  }
  else if (x + y == king->x() + king->y()) // Same diag (Diag B)
  {
    return discoverDiagB(x, y, player, king);
  }
  return nullptr;
}


bool Game::isMate() noexcept
{
  Piece* piece;
  for (int i = 0; i < SIZE; ++ i)
  {
    for (int j = 0; j < SIZE; ++ j)
    {
      piece = _board[i][j];
      if (piece != nullptr and piece->player() == _turn and piece->read().size())
      {
        return false;
      }
    }
  }
  return true;
}


Piece* Game::promote(Piece* piece, int promotion)
{
  Piece* newPiece;
  int x = piece->x(), y = piece->y();
  switch (promotion)
  {
    case 1:
      newPiece = new Queen(piece->player(), piece->x(), piece->y(), this);
      break;
    case 2:
      newPiece = new Rook(piece->player(), piece->x(), piece->y(), this);
      break;
    case 3:
      newPiece = new Bishop(piece->player(), piece->x(), piece->y(), this);
      break;
    case 4:
      newPiece = new Knight(piece->player(), piece->x(), piece->y(), this);
      break;
    default:
      throw runtime_error("Unknown promotion");
      break;
  }
  _board[y][x] = newPiece;
  delete piece;

  return newPiece;
}


bool Game::operator==(const Game& other) const
{
  return (other._turn == this->_turn and other._board == this->_board);
}


Game& Game::operator++()
{
  _turn = not _turn;
  ++ _index;
  return *this;
}


bool Game::knightNear(Piece* piece)
{
  Piece* target;
  int targetX, targetY;
  for (const vector<int>& knighMove : KNIGHT_MOVES)
  {
    targetX = piece->x() + knighMove[0];
    targetY = piece->y() + knighMove[1];
    target = at(targetX, targetY);
    if (target != nullptr and target->isKnight() and target->player() != piece->player())
    {
      return true;
    }
  }
  return false;
}


bool Game::rookAndQueenNear(Piece* piece)
{
  return false; // TODO
}


bool Game::bishopAndQueenNear(Piece* piece)
{
  return false; // TODO
}


bool Game::pawnNear(Piece* piece)
{
  return false; // TODO
}


bool Game::kingNear(Piece* piece)
{
  return false; // TODO
}