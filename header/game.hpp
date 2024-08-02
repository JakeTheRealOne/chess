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

  // #### Methods: ####

  /**
   * @brief To display the board of the chess game
   * 
   */
  void display() const noexcept;

  /**
   * @brief To display the board with available moves for the piece at (x, y)
   * 
   * @param x The x pos of the chess piece
   * @param y The y pos of the chess piece
   */
  void showMoves(const int x, const int y) noexcept;

  // #### Operators: ####

  /**
   * @brief The equality operator
   * 
   * @param other The other Game object
   * @return true If this == other
   * @return false If this != other
   */
  bool operator==(const Game& other) const;

private:
  // #### Attributes: ####
  vector<vector<Piece*>> _board;
  vector<vector<bool>> _drawingBox; //< To display moves, check etc.
  vector<Piece*> _checkList; //< The list of piece checking current player
  bool _turn = 0; //< The player that is gonna play (0: White, 1: Black)
  King* whiteKing;
  King* blackKing;
};