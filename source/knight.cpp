/**
 * @file knight.cpp
 * @author JakeTheRealOne (jakelevrai@outlook.be)
 * @brief Source file for the Knight class
 * @version 0.1
 * @date 01-08-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */


// #### Std inclusions: ####
# include <iostream>
using namespace std;

// #### Internal inclusions: ####
# include "../header/knight.hpp"


ostream& operator<<(ostream& stream, const Knight& me)
{
  stream << "Knight(" << (me._player ? "black" : "white")
         << ", " << me._x << ", " << me._y << ")";
  return stream;
}