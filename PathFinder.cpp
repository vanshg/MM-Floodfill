//
// Created by Vansh on 2/19/17.
//

#include <iostream>
#include <stack>
#include <limits.h>
#include <array>
#include "Maze.h"
#include "PathFinder.h"
#include <utility>

using namespace std;
using namespace MazeDefinitions;

bool pause;
bool justTurned = false;
std::array<std::array<int, MAZE_LEN>, MAZE_LEN> manhattanDistances;
//    int manhattanDistances[MAZE_LEN][MAZE_LEN];

stack<pair<int, int>> cellsToVisit;

PathFinderImpl::PathFinderImpl(bool shouldPause) {
    pause = shouldPause;
    initManhattanDistances();
}

MouseMovement PathFinderImpl::nextMovement(unsigned x, unsigned y, Maze &maze) {
    pauseIfNecessary();
    cout << maze.draw(5) << endl << endl;
    int currentDistance = manhattanDistances[(MAZE_LEN - 1) - y][x];
    if (currentDistance == 0) {
        cout << "Found center! Good enough for the demo, won't try to get back." << endl;
        return Finish;
    }

    int currX = 0;
    int dirToGo = 0;
    int currY = 0;
    int currDist = 0;
    if (!justTurned) {
        cellsToVisit.push(make_pair(x, y));
        while (!cellsToVisit.empty()) {
            pair<int, int> curr = cellsToVisit.top();
            cellsToVisit.pop();
            currX = curr.first;
            currY = curr.second;
            currDist = manhattanDistances[(MAZE_LEN - 1) - currY][currX];

            int minDist = INT_MAX;
            Dir currDir = maze.getCurrentDirection();
            maze.setCurrentDirection(NORTH);
            if (currX + 1 < MAZE_LEN && !maze.wallOnRight()) {
                if (manhattanDistances[MAZE_LEN - 1 - currY][currX + 1] < minDist) {
                    minDist = manhattanDistances[MAZE_LEN - 1 - currY][currX + 1];
                }
            }
            if (currX - 1 >= 0 && !maze.wallOnLeft()) {
                if (manhattanDistances[MAZE_LEN - 1 - currY][currX - 1] < minDist) {
                    minDist = manhattanDistances[MAZE_LEN - 1 - currY][currX - 1];
                }
            }
            if (currY + 1 < MAZE_LEN && !maze.wallInFront()) {
                if (manhattanDistances[MAZE_LEN - 1 - currY + 1][currX] < minDist) {
                    minDist = manhattanDistances[MAZE_LEN - 1 - (currY + 1)][currX];
                }
            }
            if (currY - 1 >= 0 && !maze.wallInBack()) {
                if (manhattanDistances[MAZE_LEN - 1 - currY - 1][currX] < minDist) {
                    minDist = manhattanDistances[MAZE_LEN - 1 - (currY - 1)][currX];
                }
            }

            maze.setCurrentDirection(currDir);
            if (minDist == INT_MAX)
                continue;
            if (currDist > minDist)
                continue;
            if (currDist <= minDist) {
                manhattanDistances[MAZE_LEN - 1 - currY][currX] = minDist + 1;
                if (currX + 1 < MAZE_LEN && !maze.wallOnRight()) {
                    cellsToVisit.push(make_pair(currX + 1, currY));
                }
                if (currX - 1 >= 0 && !maze.wallOnLeft()) {
                    cellsToVisit.push(make_pair(currX - 1, currY));
                }
                if (currY + 1 < MAZE_LEN && !maze.wallInFront()) {
                    cellsToVisit.push(make_pair(currX, currY + 1));
                }
                if (currY - 1 >= 0 && !maze.wallInBack()) {
                    cellsToVisit.push(make_pair(currX, currY - 1));
                }
            }
        }

        // flood fill is done now, so we need to pick the smallest manhattan distance path
        int minDistance = INT_MAX;
        if (x + 1 < MAZE_LEN && !maze.wallOnRight()) {
            if (manhattanDistances[MAZE_LEN - 1 - y][x + 1] < minDistance) {
                minDistance = manhattanDistances[MAZE_LEN - 1 - y][x + 1];
                dirToGo = 1;
            }
        }
        if (x - 1 >= 0 && !maze.wallOnLeft()) {
            if (manhattanDistances[MAZE_LEN - 1 - y][x - 1] < minDistance) {
                minDistance = manhattanDistances[MAZE_LEN - 1 - y][x - 1];
                dirToGo = 2;
            }
        }
        if (y + 1 < MAZE_LEN && !maze.wallInFront()) {
            if (manhattanDistances[MAZE_LEN - 1 - (y + 1)][x] < minDistance) {
                minDistance = manhattanDistances[MAZE_LEN - 1 - (y + 1)][x];
                dirToGo = 3;
            }
        }
//    if (maze.wallInFront() && maze.wallOnLeft() && maze.wallOnRight() && !maze.wallInBack()) {
//        if (manhattanDistances[x][y - 1] < minDistance) {
//            minDistance = manhattanDistances[x][y - 1];
//        }
//        dirToGo = 4;
//        cellsToVisit.push(make_pair(x, y - 1));
//    }

        if (y - 1 >= 0 && !maze.wallInBack()) {
            if (manhattanDistances[MAZE_LEN - 1 - (y - 1)][x] < minDistance) {
                minDistance = manhattanDistances[MAZE_LEN - 1 - (y - 1)][x];
                dirToGo = 4;
            }
        }
    }
    else{
        dirToGo = 0;
        justTurned = false;
    }

    cout << dirToGo << endl;

    for (int i = 0; i < MAZE_LEN; i++) {
        for (int j = 0; j < MAZE_LEN; j++) {
            cout << manhattanDistances[i][j] << "  ";
        }
        cout << endl;
    }

    switch (dirToGo) {
        case 0:
            return MoveForward;
        case 1:
            justTurned = true;
            return TurnClockwise;
        case 2:
            justTurned = true;
            return TurnCounterClockwise;
        case 3:
            return MoveForward;
        case 4:
            justTurned = true;
            return TurnAround;
        default:
            return Wait;
    }
}

bool PathFinderImpl::isAtCenter(unsigned x, unsigned y) const {
    unsigned midpoint = MAZE_LEN / 2;

    if (MAZE_LEN % 2 != 0) {
        return x == midpoint && y == midpoint;
    }

    return (x == midpoint && y == midpoint) ||
           (x == midpoint - 1 && y == midpoint) ||
           (x == midpoint && y == midpoint - 1) ||
           (x == midpoint - 1 && y == midpoint - 1);
}

void PathFinderImpl::pauseIfNecessary() {
    // Pause at each cell if the user requests it.
    // It allows for better viewing on command line.
    if (pause) {
        cout << "Hit enter to continue..." << endl;
        cin.ignore(10000, '\n');
        cin.clear();
    }
}

void PathFinderImpl::initManhattanDistances() {
    manhattanDistances = {{
                                  {{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}},
                                  {{13, 12, 11, 10, 9, 8, 7, 6, 6, 7, 8, 9, 10, 11, 12, 13}},
                                  {{12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12}},
                                  {{11, 10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9, 10, 11}},
                                  {{10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9, 10}},
                                  {{9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9}},
                                  {{8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8}},
                                  {{7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7}},
                                  {{7, 6, 5, 4, 3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 6, 7}},
                                  {{8, 7, 6, 5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 6, 7, 8}},
                                  {{9, 8, 7, 6, 5, 4, 3, 2, 2, 3, 4, 5, 6, 7, 8, 9}},
                                  {{10, 9, 8, 7, 6, 5, 4, 3, 3, 4, 5, 6, 7, 8, 9, 10}},
                                  {{11, 10, 9, 8, 7, 6, 5, 4, 4, 5, 6, 7, 8, 9, 10, 11}},
                                  {{12, 11, 10, 9, 8, 7, 6, 5, 5, 6, 7, 8, 9, 10, 11, 12}},
                                  {{13, 12, 11, 10, 9, 8, 7, 6, 6, 7, 8, 9, 10, 11, 12, 13}},
                                  {{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}},
                          }};
}
