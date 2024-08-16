/**
 * @file piece.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Piece class
 * @version 0.1
 * @date 01-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


# pragma once


// #### Std inclusions: ####
# include <iostream>
# include <vector>
using namespace std;


class Game; //< Cannot use game.hpp because of circular import


/**
 * @brief The class Piece represents a piece in a chess game
 * 
 */
class Piece
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new Piece object
   * 
   */
  Piece() noexcept = default;

  /**
   * @brief Construct a new Piece object
   * 
   * @param player The player that owns the chess piece
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @param game The game where the chess piece is located
   */
  Piece(const bool player, const int x, const int y, Game* game);

  // #### Destructor: ####

  /**
   * @brief Destroy the Knight object
   * 
   */
  virtual ~Piece() noexcept = default;

  // #### Getters: ####

  /**
   * @brief To get the player that owns the chess piece
   * 
   * @return bool 0 if White, 1 if Black
   */
  bool player() const noexcept;

  /**
   * @brief To get the x position (row) of the chess piece
   * 
   * @return int Row index
   */
  int x() const noexcept;

  /**
   * @brief To get the y position (col) of the chess piece
   * 
   * @return int Column index
   */
  int y() const noexcept;

  /**
   * @brief To get the representation of the chess piece
   * 
   * @return char 
   */
  char repr() const noexcept;

  /**
   * @brief To get a pointer to the game where the chess piece
   * is sitting
   * 
   * @return Game* Pointer to the Game object
   */
  Game* game() const noexcept;

  // #### Flags: ####

  /**
   * @brief To get the King flag
   * 
   * @return true If the piece is a king
   * @return false Else
   */
  virtual bool isKing() const noexcept;

  /**
   * @brief To get the Queen flag
   * 
   * @return true If the piece is a queen
   * @return false Else
   */
  virtual bool isQueen() const noexcept;

  /**
   * @brief To get the Rook flag
   * 
   * @return true If the piece is a rook
   * @return false Else
   */
  virtual bool isRook() const noexcept;

  /**
   * @brief To get the Knight flag
   * 
   * @return true If the piece is a knight
   * @return false Else
   */
  virtual bool isKnight() const noexcept;

  /**
   * @brief To get the Bishop flag
   * 
   * @return true If the piece is a bishop
   * @return false Else
   */
  virtual bool isBishop() const noexcept;

  /**
   * @brief To get the Pawn flag
   * 
   * @return true If the piece is a pawn
   * @return false Else
   */
  virtual bool isPawn() const noexcept;


  // #### Setters: ####

  /**
   * @brief Check if the move is legal and execute it
   *
   * @param x The new X pos to assign to the chess piece
   * @param y The new Y pos to assign to the chess piece
   */
  virtual void move(const int x, const int y) noexcept;

  /**
   * @brief Simulate the move to (x, y)
   *
   * @param x The new X pos to assign to the chess piece
   * @param y The new Y pos to assign to the chess piece
   */
  void simulateMove(const int x, const int y) noexcept;

  // #### Methods: ####

  /**
   * @brief To get the list of legal moves for the piece
   *
   * @throw std::runtime_error Because this method cannot be used on an empty piece
   * 
   * @return vector<vector<int, int>> The list of pos that the chess piece can hop on
   */
  virtual vector<vector<int>> read();

  /**
   * @brief Return if the piece threatens another piece
   * 
   * @return false Because an empty piece cannot threat any other piece
   */
  virtual bool threat(Piece*);

  // #### Operators: ####

  /**
   * @brief The equality operator
   * 
   * @param other The other Piece object
   * @return true If this == other
   * @return false If this != other
   */
  bool operator==(const Piece& other) const noexcept;

  /**
   * @brief The extraction operator
   * 
   * @param stream The stream to modify with the Piece object
   * @param me The Piece object
   * @return ostream& The modified stream
   */
  friend ostream& operator<<(ostream& stream, const Piece& me);

protected:
  // #### Attributes: ####
  bool _player = 0;  //< Owner of the chess piece.
  int _x = 0, _y = 0;  //< Position of the piece on the 8*8 board.
  int _savedIndex = -1; //> Used to identify the saved moves.
  vector<vector<int>> _savedMoves; //> Used to prevent redondant computing.
  char _repr = '?';
  Game* _game = nullptr;
};