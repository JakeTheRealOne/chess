/**
 * @file tui.hpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Header file for the TUI class
 * @version 0.1
 * @date 08-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


# pragma once


// #### Ncurses inclusion: ####
# include <ncurses.h>

// #### Internal inclusions: ####
# include "../header/game.hpp"

// #### Std inclusions: ####
# include <vector>
# include <unordered_set>
using namespace std;


/**
 * @brief The TUI class represent the TUI of a chess game (everything on screen)
 * 
 */
class TUI
{
public:
  // #### Constructors: ####

  /**
   * @brief Construct a new TUI object
   * @throw std::invalid_argument The default constructor, without any chess game, is not supported
   * 
   */
  TUI();

  /**
   * @brief Construct a new TUI object
   * 
   * @param game A game of chess
   */
  TUI(Game* game);

  // #### Destructor: ####

  /**
   * @brief Destroy the TUI object
   * 
   */
  ~TUI();

  // #### Getters: ####

  /**
   * @brief Get the X pos of the cursor
   * 
   */
  int x() const noexcept;

  /**
   * @brief Get the Y pos of the cursor
   * 
   */
  int y() const noexcept;

  // #### Methods: ####

  /**
   * @brief Reset the on-screen elements and reprint the screen
   * 
   */
  void show() const noexcept;

  /**
   * @brief Resize the TUI to center the board on the screen
   * 
   */
  static void resizeWindow(int sig) noexcept;

  /**
   * @brief Compute the screen size and x, y offset for display
   * 
   */
  void computeScreenSize() noexcept;

  /**
   * @brief Move any piece on the chess board
   * 
   * @param oldX The old X pos
   * @param oldY The old Y pos
   * @param newX The new X pos
   * @param newY The new Y pos
   */
  void move(const int oldX, const int oldY, const int newX, const int newY) noexcept;

  /**
   * @brief Show an help message (explains controls)
   * 
   * @param type The type of the help message
   *   0 = Piece selection
   *   1 = Target selection
   */
  void showHelp(const bool type) const noexcept;

  /**
   * @brief Show any message above the board
   * 
   * @param message The message (lenght < 17) to display
   */
  void showMessage(const string& message) const noexcept;

  /**
   * @brief Show the moves of the chess piece at pos (x, y)
   * 
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @return bool If there was any moves to show
   */
  bool showMoves(const int x, const int y) noexcept;

  /**
   * @brief Clean the moves on the board showed with the last TUI::showMoves
   * @note This method should always be called after each showMoves
   * 
   */
  void clearMoves() noexcept;

  /**
   * @brief move the cursor in one of the 4 direction (N S W E)
   * 
   * @param direction Between 0 and 3, is the direction of the move (N S W E)
   */
  void moveCursor(const int direction) noexcept;

  /**
   * @brief Update the case at (x, y)
   * 
   * @param x The X pos of the case
   * @param y The Y pos of the case
   * @param isCursor The flag that indiquates if the cursor is on the pos (x, y)
   */
  void update(const int x, const int y, const bool isCursor = false) const noexcept;

  /**
   * @brief Ask to the current player to select a piece
   * 
   * @return int A magic value between 0 and 4
   *  - 0: Abort (no piece selected)
   *  - 1: Promote the Pawn into a Queen
   *  - 2: Promote the Pawn into a Rook
   *  - 3: Promote the Pawn into a Bishop
   *  - 4: Promote the Pawn into a Knight
   */
  int askPromotion() const noexcept;

private:
  // #### Attributes: ####
  Game* _game;
  vector<unordered_set<int>> _moves; //< Remember moves used in showMoves
  int _x = 0, _y = 0; //< Cursor position
  int _screenWidth, _screenHeight; //< Screen size
  int _xOffset, _yOffset;


  // #### auxiliary methods: ####

  /**
   * @brief Initialize the variables of the TUI
   * 
   */
  void initVars() noexcept;

  /**
   * @brief Initialize the colors of the TUI
   * 
   */
  void initColors() noexcept;

  /**
   * @brief Initialize the signal for terminal size
   * 
   */
  void initSignal() noexcept;

};