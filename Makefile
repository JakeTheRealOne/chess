# Author: JakeTheRealOne (jakelevrai@outlook.be)
# Date: July 2024
# Goal: compile the Chess C++ project

CC := g++
FLAGS := -std=c++17 -Wall -Wextra -Wpedantic -D_GNU_SOURCE -Werror=all -O2
ICU_FLAGS := -licuuc -licudata -licui18n
COMPILE := $(CC) $(FLAGS)

OBJ_DIR := object
SRC_DIR := source
HEAD_DIR := header

all: