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

int main()
{
  Game game;
  char x, y;
  while (true)
  {
    // game.display();
    cout << "Select x and y" << endl << "x: ";
    cin >> x;
    cout << "y: ";
    cin >> y;

    x -= 48;
    y -= 48;
    if (x >= 0 and y >= 0 and x < 8 and y < 8)
    {
      game.showMoves(x, y);
    }
  }
  return 0;
}