cmake_minimum_required(VERSION 3.12)
project(IterativeDeepingAStar C)

set(CMAKE_C_STANDARD 99)

add_executable(IterativeDeepingAStar main.c Puzzle.c Puzzle.h IDA.c IDA.h Utils.c Utils.h)