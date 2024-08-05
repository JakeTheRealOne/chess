# Author: JakeTheRealOne (jakelevrai@outlook.be)
# Date: July 2024
# Goal: compile the Chess C++ project

CC := g++
FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all -O2
ICU_FLAGS := -licuuc -licudata -licui18n
NCURSES_FLAGS := -lncurses
COMPILE := $(CC)# $(FLAGS)

OBJ_DIR := object
SRC_DIR := source
HEAD_DIR := header

all:
	$(COMPILE) -c source/piece.cpp -o object/piece.o
	$(COMPILE) -c source/knight.cpp -o object/knight.o
	$(COMPILE) -c source/pawn.cpp -o object/pawn.o
	$(COMPILE) -c source/queen.cpp -o object/queen.o
	$(COMPILE) -c source/bishop.cpp -o object/bishop.o
	$(COMPILE) -c source/rook.cpp -o object/rook.o
	$(COMPILE) -c source/king.cpp -o object/king.o
	$(COMPILE) -c source/game.cpp -o object/game.o
	$(COMPILE) -c source/getkey.cpp -o object/getkey.o $(NCURSES_FLAGS)
	$(COMPILE) object/piece.o object/knight.o object/pawn.o object/queen.o object/bishop.o object/rook.o object/king.o object/game.o object/getkey.o source/main.cpp -o output $(NCURSES_FLAGS)