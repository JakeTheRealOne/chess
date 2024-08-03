/**
 * @file pawn.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the Pawn class
 * @version 0.1
 * @date 02-08-2024
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
 * @brief The Pawn class represents a pawn piece in a chess game
 * 
 */
class Pawn : public Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new Pawn object
   * 
   */
  Pawn();

  /**
   * @brief Construct a new Pawn object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   */
  Pawn(const bool player, const int x, const int y, Game* game);

  // #### Flags: ####

  /**
   * @brief To get the Pawn flag
   * 
   * @return true
   */
  bool isPawn() const noexcept override;

  /**
   * @brief To get the didntMove flag
   * 
   * @return true If the pawn is still on his 
   * @return false 
   */
  bool didntMove() const noexcept;

  // #### Setters: ####

  /**
   * @brief Check if the move is legal and execute it
   *
   * @param x The new X pos to assign to the chess piece
   * @param y The new Y pos to assign to the chess piece
   */
  void move(const int x, const int y) override;

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the pawn
   * 
   * @return vector<vector<int, int>> The list of pos that the pawn can hop on
   */
  vector<vector<int>> read() noexcept override;

  // #### Operator: ####

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the Pawn object
   * @param me The Pawn object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const Pawn& me);

private:
  // #### Attributes: ####
  bool _didntMove = true;
};