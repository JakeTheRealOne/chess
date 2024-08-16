/**
 * @file king.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the King class
 * @version 0.1
 * @date 02-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


# pragma once


// #### Internal inclusions: ####
# include "../header/piece.hpp"
# include "../header/rook.hpp"

// #### Std inclusions: ####
# include <iostream>
# include <vector>
using namespace std;


/**
 * @brief The King class represents a king piece in a chess game
 * 
 */
class King : public Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new King object
   * 
   */
  King();

  /**
   * @brief Construct a new King object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   * @param didntMove The flag didntMove of the king
   */
  King(const bool player, const int x, const int y, Game* game, bool didntMove = true);

  // #### Flags: ####

  /**
   * @brief To get the King flag
   * 
   * @return true
   */
  bool isKing() const noexcept override;

  /**
   * @brief To get the didntMove flag
   * 
   * @return true If the king hasnt moved yet
   * @return false Else
   */
  bool didntMove() const noexcept;

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the king
   * 
   * @return vector<vector<int, int>> The list of pos that the king can hop on
   */
  vector<vector<int>> read() noexcept override;

  /**
   * @brief Return if the king threatens another piece
   * 
   * @param piece The target piece
   * @return bool
   */
  bool threat(Piece* piece) override;

  /**
   * @brief Move the king to (x, y)
   *
   * @param x The new X pos to assign to the chess piece
   * @param y The new Y pos to assign to the chess piece
   */
  void move(const int x, const int y) noexcept override;

  /**
   * @brief Check for the short and long castling and add them
   * to _savedMoves if they are legal
   * 
   */
  void readCastling() noexcept;

  // #### Operator: ####

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the King object
   * @param me The King object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const King& me);

private:
  // #### Attributes: ####
  bool _didntMove = true;

  /**
   * @brief Helper of King::readCastling, return if the castling is legal
   * @note This method does not check: K and R didnt move, K not in check, R exists 
   *
   * @param rook The rook involved in the castling
   * @return bool
   */
  bool castlingLegal(Rook* rook) const noexcept;
};