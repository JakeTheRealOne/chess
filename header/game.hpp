/**
 * @file game.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the Game class
 * @version 0.1
 * @date 29-07-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


# pragma once


// #### Std inclusions: ####
# include <iostream>
# include <vector>
using namespace std;

// ### Internal inclusions: ####
# include "../header/piece.hpp"
# include "../header/knight.hpp"
# include "../header/king.hpp"


/**
 * @brief The Game class represents a game of chess
 * 
 */
class Game
{
public:
  // #### Public attributes: ####
  const int SIZE = 8;

  // #### Constructors: ####

  /**
   * @brief Construct a new Game object
   * 
   */
  Game();

  /**
   * @brief Construct a new Game object
   * @note Read backup file to play an old saved game 
   *
   * @param path The path of the backup file
   */
  Game(string path);

  // #### Destructor: ####

  /**
   * @brief Destroy the Game object (delete all pieces that was created in the heap)
   * 
   */
  ~Game();

  // #### Getters: ####

  /**
   * @brief To get the board of the game
   * 
   * @return vector<vector<Piece*>> 
   */
  vector<vector<Piece*>> board() const noexcept;

  /**
   * @brief To get the player that has to play
   * 
   */
  bool turn() const noexcept;

  /**
   * @brief To get the chess piece at pos (x, y)
   * 
   * @param x The x pos (col) of the piece
   * @param y The y pos (row) of the piece
   * @return Piece* Pointer to the piece (nullptr if the pos is empty)
   */
  Piece* at(const int x, const int y) const noexcept;

  /**
   * @brief To get the pieces that are checking the current king
   * 
   * @return vector<Piece*>
   */
  vector<Piece*> checkList() const noexcept;

  /**
   * @brief To get the king of a player
   * 
   * @param player The owner of the king (0: White, 1: Black)
   * @return Piece* Pointer to the king
   */
  Piece* king(const  bool player) const noexcept;

  /**
   * @brief To get the index of the game (the current progress)
   * 
   * @return int 
   */
  int index() const noexcept;

  // #### Methods: ####

  /**
   * @brief Move the piece to a new position
   * 
   * @param piece The involved piece
   * @param x The x of target position
   * @param y The y of target position
   * @param force Indiquate if we force the move
   */
  bool move(Piece* piece, const int x, const int y, const bool force = false) noexcept;

  /**
   * @brief Filter the vector moves to only keeps the legal ones
   * 
   * @param piece The piece that owns the moves
   * @param moves The vector of pos {x, y} for each moves
   */
  void filterMoves(Piece* piece, vector<vector<int>>& moves) const noexcept;

  /**
   * @brief Return if there is no more legal move to play for current player
   * 
   * @return bool
   */
  bool isMate() noexcept;

  /**
   * @brief After a move, update _checkList with new checks
   * @note Has to be called after Game::move
   * 
   * @param piece The involved piece
   * @param x The x of initial position
   * @param y The y of initial position
   */
  void updateCheckList(Piece* piece, const int x, const int y) noexcept;

  /**
   * @brief Return if (x, y) are between its king and a threat (ROW, COL, DIAG)
   * 
   * @param x The X pos
   * @param y The Y pos
   * @param player The owner of the king 
   * @return Piece* The threat (if there is none, nullptr)
   */
  Piece* isDiscoveryCheck(const int x, const int y, const bool player) const noexcept;

  /**
   * @brief Promote a chess piece
   * @throw std::runtime_error If the player try to abort 
   *
   * @param piece The involved piece
   * @param promotion Magic value between 0 and 4 (see TUI::askPromotion in header/tui.hpp)
   * @return The pointer of the new piece (after promotion)
   */
  Piece* promote(Piece* piece, int promotion);

  /**
   * @brief To remove all moves that are illegal from moves of a piece
   * 
   * @param moves The list of moves given
   * @param piece The piece that want to move
   */
  void filterMoves(vector<vector<int>>& moves, Piece* piece) noexcept;

  // #### Operators: ####

  /**
   * @brief The equality operator
   * 
   * @param other The other Game object
   * @return true If this == other
   * @return false If this != other
   */
  bool operator==(const Game& other) const;

  /**
   * @brief The increment operator
   * 
   * @return Game& This game
   */
  Game& operator++();

private:
  // #### Attributes: ####
  vector<vector<Piece*>> _board;
  vector<Piece*> _checkList; //< The list of piece checking current player
  bool _turn = 0; //< The player that is gonna play (0: White, 1: Black)
  King* _whiteKing;
  King* _blackKing;
  int _index = 0; //< The index of the current move in the game (from 0 to inf.)

  // #### Auxiliary methods: ####

  /**
   * @brief Helper for isDiscoveryCheck, check for possible threat on the same row as the king
   * 
   * @param x The X pos
   * @param y The Y pos
   * @param player The owner of the king
   * @param king The king that might be in check
   * @return Piece* The piece that is checking the king (or nullptr if none)
   */
  Piece* discoverRow(const int x, const int y, const bool player, Piece* king) const noexcept;

  /**
   * @brief Helper for isDiscoveryCheck, check for possible threat on the same column as the king
   * 
   * @param x The X pos
   * @param y The Y pos
   * @param player The owner of the king
   * @param king The king that might be in check
   * @return Piece* The piece that is checking the king (or nullptr if none)
   */
  Piece* discoverCol(const int x, const int y, const bool player, Piece* king) const noexcept;

  /**
   * @brief Helper for isDiscoveryCheck, check for possible threat on the same diagonal (A) as the king
   * 
   * @param x The X pos
   * @param y The Y pos
   * @param player The owner of the king
   * @param king The king that might be in check
   * @return Piece* The piece that is checking the king (or nullptr if none)
   */
  Piece* discoverDiagA(const int x, const int y, const bool player, Piece* king) const noexcept;

  /**
   * @brief Helper for isDiscoveryCheck, check for possible threat on the same diagonal (B) as the king
   * 
   * @param x The X pos
   * @param y The Y pos
   * @param player The owner of the king
   * @param king The king that might be in check
   * @return Piece* The piece that is checking the king (or nullptr if none)
   */
  Piece* discoverDiagB(const int x, const int y, const bool player, Piece* king) const noexcept;

  /**
   * @brief Helper for filterMoves, erase illegals moves for piece, the king
   * 
   * @param piece The king
   * @param moves The moves of the king (vector of pos {x, y})
   */
  void filterKingMoves(Piece* piece, vector<vector<int>>& moves) noexcept;

  /**
   * @brief Helper for filterMoves, erase illegals moves for piece, that is not the king
   * 
   * @param piece The involved chess piece
   * @param moves The moves of the king (vector of pos {x, y})
   */
  void filterNotKingMoves(Piece* piece, vector<vector<int>>& moves) noexcept;
};