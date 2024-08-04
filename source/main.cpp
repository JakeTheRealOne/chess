/**
 * @file main.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief main Source file
 * @version 0.1
 * @date 29-07-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Internal inclusions: ####
# include "../header/main.hpp"
# include "../header/game.hpp"

// #### Std inclusions: ####
# include <iostream>
using namespace std;


void clearTerminal()
{
  system("clear");
}


void run(Game& game)
{
  char x, y;
  do {
    clearTerminal();
    game.display();
    cout << (game.turn() ? "Black" : "White") << " to play, select a case: " << endl << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;
    x -= 48, y -= 48;
  }
  while (x >= 0 and x < 8 and y >= 0 and y < 8);
  
}


int main()
{
  Game game;
  char x, y;
  while (true)
  {
    cout << "TURN N*" << game.index() << endl;
    cout << endl;
    game.display();
    cout << "Select x and y" << endl << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;

    x -= 48;
    y -= 48;
    if (game.at(x, y) != nullptr)
    {
      cout << endl;
      game.showMoves(x, y);
      Piece* piece = game.at(x, y);
      cout << "Select target x and y" << endl << "x: ";
      cin >> x;
      cout << "y: ";
      cin >> y;
      x -= 48;
      y -= 48;
      if (x >= 0 and x < 8 and y >= 0 and y < 8)
      {
        game.move(piece, x, y);
        ++ game;
      }
    }
  }
  return 0;
}