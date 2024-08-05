/**
 * @file getkey.hpp
 * @author your name (you@domain.com)
 * @brief Header file for the getkey script
 * @version 0.1
 * @date 04-08-2024
 *
 * @copyright Copyright (c) 2024
 *
 */


# pragma once


/**
 * @brief Read keyboard inputs to get the player decisions
 *
 * @note I do not initiate ncurses, please do it yourself in your TUI
 *
 * @return int A magic value between -1 and 5:
 *  -1: Unknown (ignore input)
 *  0: Escape key (abort current move)
 *  1: Enter key (confirm current move)
 *  2: Up key (decrement the y of current position)
 *  3: Down key (increment the y of current position)
 *  4: Left key (decrement the x of current position)
 *  5: Right key (increment the x of current position)
 */
int getkey();
