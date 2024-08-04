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

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the bishop
   * 
   * @return vector<vector<int, int>> The list of pos that the bishop can hop on
   */
  vector<vector<int>> read() noexcept override;

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
   * @brief To check if the case (x, y) is a valid target for the bishop
   * 
   * @param x The x pos of the target
   * @param y The y pos of the target
   * @return true If the bishop can continue
   * @return false If the bishop hits a bound (end of grid, other piece, etc.)
   */
  bool checkTarget(const int x, const int y) noexcept;
};