# Author: JakeTheRealOne (jakelevrai@outlook.be)
# Date: July 2024
# Goal: compile the Chess C++ project

CC := g++
FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all -O2
ICU_FLAGS := -licuuc -licudata -licui18n
NCURSES_FLAGS := -lncurses

OBJ_DIR := object
SRC_DIR := source
HEAD_DIR := header

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
EXEC := chess

all: create_obj_dir $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@ $(NCURSES_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)

create_obj_dir:
	mkdir -p object