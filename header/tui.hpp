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
# include <string>
using namespace std;


/**
 * @brief The TUI class represent the TUI of a chess game (everything on screen)
 * @note The TUI class do not modify the game values (pos, etc.), It just sample them to
 * display them on the screen
 * 
 */
class TUI
{
public:
  // #### Public attributes: ####
  const vector<string> PROMOTION_OPTIONS = {"  Queen         ", "  Rook          ", "  Bishop        ", "  Knight        "};
  const vector<string> MENU_OPTIONS =      {"    New game    ", "    Load game   ", "     Themes     ", "    Controls    "};
  const vector<string> LOGO = {"       _                   ", "   ___| |__   ___  ___ ___ ", "  / __| '_ \\ / _ \\/ __/ __|", " | (__| | | |  __/\\__ \\__ \\", "  \\___|_| |_|\\___||___/___/"};
  const int LOGO_WIDTH = 27, LOGO_HEIGHT = 5, THEMES = 4;

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

  // #### Setters: ####

  /**
   * @brief Set the game of the TUI
   * @throw std::invalid_argument if the pointer is nullptr
   * 
   * @param newGame The new game of the TUI
   */
  void setGame(Game* newGame);

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
   *   2 = Pawn promotion
   */
  void showHelp(const short type) const noexcept;

  /**
   * @brief Show the chess logo
   * 
   */
  void showLogo() const noexcept;

  /**
   * @brief Show any message above the board
   * 
   * @param message The message (lenght < 17) to display
   */
  void showMessage(const string& message) const noexcept;

  /**
   * @brief Display the main menu
   * 
   */
  void showMenu() const noexcept;

  /**
   * @brief Display the theme menu
   * 
   */
  void showThemes() const noexcept; 

  /**
   * @brief Show the moves of the chess piece at pos (x, y)
   * 
   * @param x The X pos of the chess piece
   * @param y The Y pos of the chess piece
   * @return bool If there was any moves to show
   */
  bool showMoves(const int x, const int y) noexcept;

  /**
   * @brief Display the promotion panel
   * 
   */
  void showPromotionPanel() const noexcept;

  /**
   * @brief Change the promotion selected in the promotion panel
   * 
   * @param index The current index
   * @param increment The increment
   * @return int The updated index
   */
  int changePromotion(int index, int increment) const noexcept;

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

  /**
   * @brief Get an option in the menu (ask to user)
   * 
   * @return int A magic value between 0 and 4
   *  - 0: Abort (Quit the program)
   *  - 1: Start a new game
   *  - 2: Load a saved game
   *  - 3: Open the theme menu
   *  - 4: Open the controls menu
   */
  int getMenuOption() const noexcept;

  /**
   * @brief Change the index of the current selected option in the menu
   * 
   * @param index The current index
   * @param increment The increment
   * @return int The updated index
   */
  int changeMenu(int index, int increment) const noexcept;

  /**
   * @brief Get an option in the menu (ask to user)
   * 
   * @return int A magic value between 0 and 4
   *  - 0: Abort (Do not change the theme)
   *  - 1: Classic
   *  - 2: Glass
   */
  int getTheme() const noexcept;

  /**
   * @brief Change the index of the current selected theme
   * 
   * @param index The current index
   * @param increment The increment
   * @param orientation The orientation of the increment (0 = vertical, 1 = horizontal)
   * @return int The updated index
   */
  int changeTheme(int index, int increment, bool orientation) const noexcept;

  /**
   * @brief read the theme from memory/theme
   * 
   * @return int I/O return code
   */
  int readTheme() const;

  /**
   * @brief Write the theme in memory/theme to remember it next time the program starts
   * 
   * @param theme The theme signature (from 0 to THEMES - 1)
   * @return int I/O return code
   */
  int writeTheme(char theme) const;

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