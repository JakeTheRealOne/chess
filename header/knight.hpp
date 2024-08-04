/**
 * @file knight.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the Knight class
 * @version 0.1
 * @date 01-08-2024
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
 * @brief The Knight class represents a knight piece in a chess game
 * 
 */
class Knight : public Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new Knight object
   * 
   */
  Knight();

  /**
   * @brief Construct a new Knight object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   */
  Knight(const bool player, const int x, const int y, Game* game);

  // #### Flags: ####

  /**
   * @brief To get the Knight flag
   * 
   * @return true
   */
  bool isKnight() const noexcept override;

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the knight
   * 
   * @return vector<vector<int, int>> The list of pos that the knight can hop on
   */
  vector<vector<int>> read() noexcept override;

  // #### Operator: ####

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the Knight object
   * @param me The Knight object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const Knight& me);
};