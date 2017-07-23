cmake_minimum_required(VERSION 3.7)
project(MM_Floodfill)

set(CMAKE_CXX_STANDARD 11)

#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
        BitVector256.h
        Dir.h
        main.cpp
        Maze.cpp
        Maze.h
        MazeDefinitions.h
        PathFinder.cpp
        PathFinder.h)

add_executable(MM_Floodfill ${SOURCE_FILES})