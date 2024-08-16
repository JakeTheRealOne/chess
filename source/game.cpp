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
# include <fstream>
# include <filesystem>
# include <unordered_map>
using namespace std;
namespace fs = filesystem;

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

  _whiteKing = new King(0, 4, 7, this);
  _blackKing = new King(1, 4, 0, this);
  _board[7][4] = _whiteKing;
  _board[0][4] = _blackKing;
}


Game::Game(string path)
{
  for (int y = 0; y < SIZE; ++ y)
  {
    _board.push_back(vector<Piece*>(SIZE, nullptr));
  }
  ifstream file(path);
  if (!file)
  {
    throw runtime_error("wrong path for save file");
  }
  file.seekg(9, ios::cur); //< skip the signature CHESSJKLV
  // Get _index
  initIndex(file);
  // Get _turn
  initTurn(file);
  // Get _50movesrule
  init50moves(file);
  // Get _board
  initBoard(file);
  // Get _checkList
  initCheckList(file);
  // Sanity check: kings must exist
  sanityCheck();
  _name = fs::directory_entry(path).path().filename();
}


Game::~Game()
{
  for (const vector<Piece*>& row : _board)
  {
    for (const Piece* piece : row)
    {
      if (piece != nullptr)
      {
        delete piece;
      }
    }
  }
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


bool Game::drawBy50Moves() const noexcept
{
  return this->_50moveRules > 49;
}


bool Game::drawByRepetition() const noexcept
{
  auto lastPosition = _repetitions.find(_hash);
  return (lastPosition != _repetitions.end()) and (lastPosition->second >= 3);
}

vector<char> Game::hash() noexcept
{
  _hash.clear();
  _hash.reserve(SIZE * SIZE + 1);
  _hash.push_back(_turn);
  for (int y = 0; y < SIZE; ++ y)
  {
    for (int x = 0; x < SIZE; ++ x)
    {
      _hash.push_back(_board[y][x] == nullptr ? 0 : _board[y][x]->repr());
    }
  }
  return _hash;
}


bool Game::move(Piece* piece, const int x, const int y, const bool force)
{
  // Check the legality of the move:
  vector<int> pos = {x, y};
  vector<vector<int>> available = (piece == nullptr ? vector<vector<int>>(/*empty list*/) : piece->read());
  bool eat = false;
  if (not force and find(available.begin(), available.end(), pos) == available.end())
  {
    return 1;
  }
  if (_board[y][x] != nullptr)
  {
    eat = true;
    delete _board[y][x];
  }
  else if (piece->isPawn() and piece->x() != x)
  {
    eat = true;
    delete _board[piece->y()][x];
    _board[piece->y()][x] = nullptr;
  }
  else if (piece->isKing() and abs(x - piece->x()) == 2)
  {
    Piece* rook = _board[piece->y()][(x - piece->x() == -2) ? 0 : SIZE - 1];
    if (rook == nullptr)
    {
      throw runtime_error("try to castle but no rook was detected");
    }
    int rookTarget = (x - piece->x() == -2) ? x + 1 : x - 1;
    _board[piece->y()][rookTarget] = rook;
    _board[rook->y()][rook->x()] = nullptr;
    rook->move(rookTarget, piece->y());

  }
  _board[y][x] = piece;
  _board[piece->y()][piece->x()] = nullptr;
  piece->move(x, y);
  if (piece->isPawn() or eat)
  {
    // Reset 50 moves rule Counting
    _50moveRules = 0;
    // Clear the repetitions hash table (things will never be like before)
    _repetitions.clear();
  }
  else
  {
    // Increment 50 moves rule Counting
    ++ _50moveRules;
  }
  hash();
  ++ _repetitions[_hash];
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
    if (piece->isPawn() and piece->y() == (piece->player() ? 4 : 3))
    {
      filterEnPassant(piece, moves);
    }
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
    _board[y][x] = nullptr;
    piece->simulateMove(moveX, moveY);
    bool notSafe = knightNear(piece) or rookAndQueenNear(piece) or bishopAndQueenNear(piece) or pawnNear(piece) or kingNear(piece);
    _board[moveY][moveX] = previous;
    _board[y][x] = piece;
    piece->simulateMove(x, y);
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
      if (
        (moveX == threat->x() and moveY == threat->y())
        or
        (piece->isPawn() and moveX != x and _board[moveY][moveX] == nullptr
        and threat->isPawn() and threat->x() == moveX and threat->y() == y)
      )
      {
        continue;
      }

      if (canBeBlocked and _board[moveY][moveX] == nullptr)
      {
        _board[moveY][moveX] = piece;
        _board[y][x] = nullptr;
        piece->simulateMove(moveX, moveY);
        bool blocked = not threat->threat(myKing);
        _board[moveY][moveX] = nullptr;
        _board[y][x] = piece;
        piece->simulateMove(x, y);
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
  Piece* pinned = isDiscoveryCheck(x, y, piece->player());
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
        piece->simulateMove(moveX, moveY);
        bool blocked = not pinned->threat(myKing);
        _board[moveY][moveX] = nullptr;
        _board[y][x] = piece;
        piece->simulateMove(x, y);
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


void Game::filterEnPassant(Piece* pawn, vector<vector<int>>& moves)
{
  // En passant
  int x = pawn->x(), moveX, moveY, n = moves.size();
  for (int i = 0; i < n; ++ i)
  {
    moveX = moves[i][0];
    moveY = moves[i][1];
    if (moveX != x and _board[moveY][moveX] == nullptr)
    {
      bool notBlocked = isDiscoveryCheck(moveX, moveY, pawn->player());
      if (notBlocked)
      {
        swap(moves[i], moves.back());
        moves.pop_back();
        -- n;
        -- i;
      }
    }
  }
}

void Game::updateCheckList(Piece* piece, const int x, const int y) noexcept
{
  _checkList.clear();
  // Direct 
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
  if (drawBy50Moves() or drawByRepetition())
  {
    return true;
  }
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
  bool output = false;
  int x = piece->x(), y = piece->y();
  if (not output and x)
  {
    if (_board[y][x - 1] != nullptr)
    {
      output = (_board[y][x - 1]->player() xor piece->player()) and (_board[y][x - 1]->isRook() or _board[y][x - 1]->isQueen());
    }
    else
    {
      output = discoverRow(x - 1, y, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and y)
  {
    if (_board[y - 1][x] != nullptr)
    {
      output = (_board[y - 1][x]->player() xor piece->player()) and (_board[y - 1][x]->isRook() or _board[y - 1][x]->isQueen());
    }
    else
    {
      output = discoverCol(x, y - 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and x < SIZE - 1)
  {
    if (_board[y][x + 1] != nullptr)
    {
      output = (_board[y][x + 1]->player() xor piece->player()) and (_board[y][x + 1]->isRook() or _board[y][x + 1]->isQueen());
    }
    else
    {
      output = discoverRow(x + 1, y, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and y < SIZE - 1)
  {
    if (_board[y + 1][x] != nullptr)
    {
      output = (_board[y + 1][x]->player() xor piece->player()) and (_board[y + 1][x]->isRook() or _board[y + 1][x]->isQueen());
    }
    else
    {
      output = discoverCol(x, y + 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  return output;
}


bool Game::bishopAndQueenNear(Piece* piece)
{
  bool output = false;
  int x = piece->x(), y = piece->y();
  if (not output and x and y)
  {
    if (_board[y - 1][x - 1] != nullptr)
    {
      output = (_board[y - 1][x - 1]->player() xor piece->player()) and (_board[y - 1][x - 1]->isRook() or _board[y - 1][x - 1]->isQueen());
    }
    else
    {
      output = discoverDiagA(x - 1, y - 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and y < SIZE - 1 and x < SIZE - 1)
  {
    if (_board[y + 1][x + 1] != nullptr)
    {
      output = (_board[y + 1][x + 1]->player() xor piece->player()) and (_board[y + 1][x + 1]->isRook() or _board[y + 1][x + 1]->isQueen());
    }
    else
    {
      output = discoverDiagA(x + 1, y + 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and x and y < SIZE - 1)
  {
    if (_board[y + 1][x - 1] != nullptr)
    {
      output = (_board[y + 1][x - 1]->player() xor piece->player()) and (_board[y + 1][x - 1]->isRook() or _board[y + 1][x - 1]->isQueen());
    }
    else
    {
      output = discoverDiagB(x - 1, y + 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  if (not output and x < SIZE - 1 and y)
  {
    if (_board[y - 1][x + 1] != nullptr)
    {
      output = (_board[y - 1][x + 1]->player() xor piece->player()) and (_board[y - 1][x + 1]->isRook() or _board[y - 1][x + 1]->isQueen());
    }
    else
    {
      output = discoverDiagB(x + 1, y - 1, piece->player(), king(piece->player())) != nullptr;
    }
  }
  return output;
}


bool Game::pawnNear(Piece* piece)
{
  int pawnY = piece->y() + (piece->player() ? +1 : -1);
  Piece* spot1 = at(piece->x() + 1, pawnY),
       * spot2 = at(piece->x() - 1, pawnY);
  return (
    (spot1 != nullptr and spot1->player() != piece->player() and spot1->isPawn())
    or
    (spot2 != nullptr and spot2->player() != piece->player() and spot2->isPawn())
  );
}


bool Game::kingNear(Piece* piece)
{
  Piece* otherKing = king(not piece->player());
  int diffX = abs(otherKing->x() - piece->x()), diffY = abs(otherKing->y() - piece->y());
  return (diffX < 2 and diffY < 2 and (diffX or diffY));
}


void Game::initTurn(ifstream& file)
{
  if (file.eof())
  {
    throw runtime_error("corrupted file (ERR_CODE: 1)");
  }
  char turn;
  file.read(&turn, sizeof(turn));
  _turn = turn;
}


void Game::initIndex(ifstream& file)
{
  if (file.eof())
  {
    throw runtime_error("corrupted file (ERR_CODE: 2)");
  }
  char index;
  file.read(&index, sizeof(index));
  _index = index;
}


void Game::init50moves(ifstream& file)
{
  if (file.eof())
  {
    throw runtime_error("corrupted file (ERR_CODE: 2)");
  }
  char fiftyMovesRule;
  file.read(&fiftyMovesRule, sizeof(fiftyMovesRule));
  _50moveRules = fiftyMovesRule;
}


void Game::initBoard(ifstream& file)
{
  for (int y = 0; y < SIZE; ++ y)
  {
    for (int x = 0; x < SIZE; ++ x)
    {
      if (file.eof())
      {
        throw runtime_error("corrupted file (ERR_CODE: 3)");
      }
      char pieceType, pieceOwner, doubleUp, didntMove;
      file.read(&pieceType, sizeof(pieceType));
      file.read(&pieceOwner, sizeof(pieceOwner));
      switch (pieceType)
      {
        case 0:
          continue;
        case 'P':
          file.read(&doubleUp, sizeof(doubleUp));
          _board[y][x] = new Pawn(pieceOwner, x, y, this, doubleUp);
          break;
        case 'R':
          file.read(&didntMove, sizeof(didntMove));
          _board[y][x] = new Rook(pieceOwner, x, y, this, didntMove);
          break;
        case 'N':
          _board[y][x] = new Knight(pieceOwner, x, y, this);
          break;
        case 'B':
          _board[y][x] = new Bishop(pieceOwner, x, y, this);
          break;
        case 'Q':
          _board[y][x] = new Queen(pieceOwner, x, y, this);
          break;
        case 'K':
          if ((pieceOwner ? _blackKing : _whiteKing) != nullptr)
          {
            throw runtime_error("corrupted file (ERR_CODE: 5)");
          }
          file.read(&didntMove, sizeof(didntMove));
          (pieceOwner ? _blackKing : _whiteKing) = new King(pieceOwner, x, y, this, didntMove);
          _board[y][x] = (pieceOwner ? _blackKing : _whiteKing);
          break;
        default:
          throw runtime_error("corrupted file (ERR_CODE: 4)");
      }
    }
  }
}


void Game::initCheckList(ifstream& file)
{
  if (file.eof())
  {
    throw runtime_error("corrupted file (ERR_CODE: 6)");
  }
  char size, x, y;
  file.read(&size, sizeof(size));
  for (int index = 0; index < size; ++ index)
  {
    file.read(&x, sizeof(x));
    file.read(&y, sizeof(y));
    if (y >= SIZE or x >= SIZE or y < 0 or x < 0 or _board[y][x] == nullptr)
    {
      throw runtime_error("corrupted file (ERR_CODE: 7)");
    }
    _checkList.push_back(_board[y][x]);
  }
}


void Game::sanityCheck()
{
  if (_whiteKing == nullptr or _blackKing == nullptr)
  {
    throw runtime_error("corrupted file (ERR_CODE: 8)");
  }
}


void Game::save()
{
  static char signature[] = {'C', 'H', 'E', 'S', 'S', 'J', 'K', 'L', 'V'};
  ofstream file("memory/saved_games/" + _name);
  if (!file)
  {
    throw runtime_error("cannot write save file");
  }
  file.write(signature, sizeof(signature));
  writeIndex(file);
  writeTurn(file);
  write50moves(file);
  writeBoard(file);
  writeCheckList(file);
}


void Game::writeTurn(ofstream& file)
{
  char turn = _turn;
  file.write(&turn, sizeof(turn));
}


void Game::writeIndex(ofstream& file)
{
  char index = _index;
  file.write(&index, sizeof(index));
}


void Game::write50moves(ofstream& file)
{
  char fiftyMovesRule = _50moveRules;
  file.write(&fiftyMovesRule, sizeof(fiftyMovesRule));
}


void Game::writeBoard(ofstream& file)
{
  Piece* piece;
  char pieceType, pieceOwner, doubleUp, didntMove;
  for (int y = 0; y < SIZE; ++ y)
  {
    for (int x = 0; x < SIZE; ++ x)
    {
      piece = _board[y][x];
      if (piece == nullptr)
      {
        pieceType = 0;
        pieceOwner = 0;
      }
      else
      {
        pieceType = piece->repr();
        pieceOwner = piece->player();
      }
      file.write(&pieceType, sizeof(pieceType));
      file.write(&pieceOwner, sizeof(pieceOwner));
      if (piece != nullptr)
      {
        if (piece->isPawn())
        {
          doubleUp = ((Pawn*)piece)->doubleUpIndex();
          file.write(&doubleUp, sizeof(doubleUp));
        }
        else if (piece->isRook())
        {
          didntMove = ((Rook*)piece)->didntMove();
          file.write(&didntMove, sizeof(didntMove));
        }
        else if (piece->isKing())
        {
          didntMove = ((King*)piece)->didntMove();
          file.write(&didntMove, sizeof(didntMove));
        }
      }
    }
  }
}


void Game::writeCheckList(ofstream& file)
{
  char size = _checkList.size(), x, y;
  file.write(&size, sizeof(size));
  for (int index = 0; index < size; ++ index)
  {
    x = _checkList[index]->x(), y = _checkList[index]->y();
    file.write(&x, sizeof(x));
    file.write(&y, sizeof(y));
  }
}


string Game::name() const noexcept
{
  return _name;
}


void Game::setName(string& newName) noexcept
{
  if (newName.size()) // Does not allow empty names
  {
    _name = newName;
  }
}
