/**
 * @file rook.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the Rook class
 * @version 0.1
 * @date 03-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


# pragma once


// #### Internal inclusions: ####
# include "../header/piece.hpp"

// #### Std inclusions: ####
# include <iostream>
# include <vector>
using namespace std;


/**
 * @brief The Rook class represents a rook piece in a chess game
 * 
 */
class Rook : public Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new Rook object
   * 
   */
  Rook();

  /**
   * @brief Construct a new Rook object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   * @param didntMove The flag didntMove of the king
   */
  Rook(const bool player, const int x, const int y, Game* game, bool didntMove = true);

  // #### Flags: ####

  /**
   * @brief To get the Rook flag
   * 
   * @return true
   */
  bool isRook() const noexcept override;

  /**
   * @brief To get the didntMove flag
   * 
   * @return true If the rook hasnt moved yet
   * @return false Else
   */
  bool didntMove() const noexcept;

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the rook
   * 
   * @return vector<vector<int, int>> The list of pos that the rook can hop on
   */
  vector<vector<int>> read() noexcept override;

  /**
   * @brief Return if the rook threatens another piece
   * 
   * @param piece The target piece
   * @return bool
   */
  bool threat(Piece* piece) override;

  /**
   * @brief Move the rook to (x, y)
   *
   * @param x The new X pos to assign to the chess piece
   * @param y The new Y pos to assign to the chess piece
   */
  void move(const int x, const int y) noexcept override;

  // #### Operator: ####

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the Rook object
   * @param me The Rook object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const Rook& me);

private:
  // #### Attributes: ####
  bool _didntMove = true;

  // #### Auxilary methods: ####

  /**
   * @brief To check if the case (x, y) is a valid target for the rook
   * 
   * @param x The x pos of the target
   * @param y The y pos of the target
   * @return true If the rook can continue
   * @return false If the rook hits a bound (end of grid, other piece, etc.)
   */
  bool checkTarget(const int x, const int y) noexcept;
};