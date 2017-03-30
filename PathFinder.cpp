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
std::array<std::array<int, MAZE_LEN>, MAZE_LEN> manhattanDistances;
//    int manhattanDistances[MAZE_LEN][MAZE_LEN];

stack<pair<int, int>> cellsToVisit;

PathFinderImpl::PathFinderImpl(bool shouldPause) {
    pause = shouldPause;
    initManhattanDistances();
    cellsToVisit.push(make_pair(0, 0));
}

MouseMovement PathFinderImpl::nextMovement(unsigned x, unsigned y, const Maze &maze) {
    cellsToVisit.pop();
    pauseIfNecessary();
    cout << maze.draw(5) << endl << endl;
    int currentDistance = manhattanDistances[x][y];
    if (currentDistance == 0 || isAtCenter(x, y)) {
        cout << "Found center! Good enough for the demo, won't try to get back." << endl;
        return Finish;
    }

    int dirToGo = 0;

    int minDistance = INT_MAX;
    if (x + 1 < MAZE_LEN && !maze.wallOnRight()) {
        cellsToVisit.push(make_pair(x + 1, y));
        if (manhattanDistances[x + 1][y] < minDistance) {
            minDistance = manhattanDistances[x + 1][y];
        }
        dirToGo = 1;
    }
    if (x - 1 >= 0 && !maze.wallOnLeft()) {
        if (manhattanDistances[x - 1][y] < minDistance) {
            minDistance = manhattanDistances[x - 1][y];
        }
        dirToGo = 2;
        cellsToVisit.push(make_pair(x - 1, y));
    }
    if (y + 1 < MAZE_LEN && !maze.wallInFront()) {
        if (manhattanDistances[x][y + 1] < minDistance) {
            minDistance = manhattanDistances[x][y + 1];
        }
        dirToGo = 3;
        cellsToVisit.push(make_pair(x, y + 1));
    }
    if (maze.wallInFront() && maze.wallOnLeft() && maze.wallOnRight() && !maze.wallInBack()) {
        if (manhattanDistances[x][y - 1] < minDistance) {
            minDistance = manhattanDistances[x][y - 1];
        }
        dirToGo = 4;
        cellsToVisit.push(make_pair(x, y - 1));
    }

//    if (y - 1 >= 0 && !maze.wallInBack()) {
//        if (manhattanDistances[x][y - 1] < minDistance) {
//            minDistance = manhattanDistances[x][y - 1];
//        }
//        dirToGo = 4;
//        cellsToVisit.push(make_pair(x, y - 1));
//    }

    if (minDistance == INT_MAX) {
        return Wait;
    }

    if (currentDistance <= minDistance) {
        manhattanDistances[x][y] = minDistance + 1;
    }

    cout << dirToGo << endl;

    switch (dirToGo) {
        case 0:
            return MoveForward;
        case 1:
            return TurnClockwise;
        case 2:
            return TurnCounterClockwise;
        case 3:
            return MoveForward;
        case 4:
            return TurnAround;
        default:
            return Wait;
    }
    return Wait;
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
