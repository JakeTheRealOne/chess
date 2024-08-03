/**
 * @file bishop.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the Bishop class
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
 * @brief The Bishop class represents a bishop piece in a chess game
 * 
 */
class Bishop : public Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new Bishop object
   * 
   */
  Bishop();

  /**
   * @brief Construct a new Bishop object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   */
  Bishop(const bool player, const int x, const int y, Game* game);

  // #### Flags: ####

  /**
   * @brief To get the Bishop flag
   * 
   * @return true
   */
  bool isBishop() const noexcept override;

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
   * @brief To get the list of legal moves for the bishop
   * 
   * @return vector<vector<int, int>> The list of pos that the bishop can hop on
   */
  vector<vector<int>> read() const noexcept override;

  // #### Operator: ####

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the Bishop object
   * @param me The Bishop object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const Bishop& me);

private:
  // #### Auxilary methods: ####

  /**
   * @brief Filter the moves given to keep only the legal ones
   * 
   */
  void filterMoves(vector<vector<int>>& moves) const noexcept;
};