cmake_minimum_required(VERSION 3.9)
project(cluedo C)

set(CMAKE_C_STANDARD 90)

add_executable(cluedo main.c BoardManager.h BoardManager.c Pieces.h Pieces.c Standards.c Standards.h Gameplay.c Gameplay.h AI.c AI.h)

