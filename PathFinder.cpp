//
// Created by Vansh on 2/19/17.
//

#include <iostream>
#include <stack>
#include <limits.h>
#include <array>
#include "PathFinder.h"
#include "Maze.h"

using namespace std;
using namespace MazeDefinitions;

class PathFinderImpl : PathFinder {
public:

    PathFinderImpl(bool shouldPause = false) : pause(shouldPause) {
        initManhattanDistances();
    }

    MouseMovement nextMovement(unsigned x, unsigned y, const Maze &maze) override {
        pauseIfNecessary();
        cout << maze.draw(5) << endl << endl;
        int currentDistance = manhattanDistances[x][y];
        if (currentDistance == 0 || isAtCenter(x, y)) {
            cout << "Found center! Good enough for the demo, won't try to get back." << endl;
            return Finish;
        }

        int minDistance = INT_MAX;
        if (x + 1 < MAZE_LEN && manhattanDistances[x + 1][y] < minDistance && !maze.wallOnRight()) {
            minDistance = manhattanDistances[x + 1][y];
//            cellsToVisit.push(pair(x + 1, y));
        }
        if (x - 1 >= 0 && manhattanDistances[x - 1][y] < minDistance && !maze.wallOnLeft()) {
            minDistance = manhattanDistances[x - 1][y];
//            cellsToVisit.push(pair(x - 1, y));
        }
        if (y + 1 < MAZE_LEN && manhattanDistances[x][y + 1] < minDistance && !maze.wallInFront()) {
            minDistance = manhattanDistances[x][y + 1];
//            cellsToVisit.push(pair(x, y + 1));
        }
        if (y - 1 >= 0 && manhattanDistances[x][y - 1] < minDistance /*&& !maze.wallInBack() ?????*/) {
            minDistance = manhattanDistances[x][y - 1];
//            cellsToVisit.push(pair(x, y - 1));
        }

        if (minDistance == INT_MAX) {
//            return Wait;
        }

        if (currentDistance > minDistance) {
//            return Wait;
        }

        if (currentDistance <= minDistance) {
            manhattanDistances[x][y] = minDistance + 1;
        }

        // Push every neighbor onto stack

        return Wait;
    }

protected:
    const bool pause;
    std::array<std::array<int, MAZE_LEN>, MAZE_LEN> manhattanDistances;
    stack<pair<int, int>> cellsToVisit;

    bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MAZE_LEN / 2;

        if (MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return (x == midpoint && y == midpoint) ||
               (x == midpoint - 1 && y == midpoint) ||
               (x == midpoint && y == midpoint - 1) ||
               (x == midpoint - 1 && y == midpoint - 1);
    }

    void pauseIfNecessary() {
        // Pause at each cell if the user requests it.
        // It allows for better viewing on command line.
        if (pause) {
            cout << "Hit enter to continue..." << endl;
            cin.ignore(10000, '\n');
            cin.clear();
        }
    }

    void initManhattanDistances() {
        manhattanDistances = {{
                {{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}},
                {{13, 12, 11, 10,  9, 8, 7, 6, 6, 7, 8,  9, 10, 11, 12, 13}},
                {{12, 11, 10, 9,  8,  7, 6, 5, 5, 6, 7, 8,  9,  10, 11, 12}},
                {{11, 10, 9,  8,  7,  6, 5, 4, 4, 5, 6, 7,  8,  9,  10, 11}},
                {{10, 9,  8,  7,  6,  5, 4, 3, 3, 4, 5, 6,  7,  8,  9,  10}},
                {{9,  8,  7,  6,  5,  4, 3, 2, 2, 3, 4, 5,  6,  7,  8,  9}},
                {{8,  7,  6,  5,  4,  3, 2, 1, 1, 2, 3, 4,  5,  6,  7,  8}},
                {{7,  6,  5,  4,  3,  2, 1, 0, 0, 1, 2, 3,  4,  5,  6,  7}},
                {{7,  6,  5,  4,  3,  2, 1, 0, 0, 1, 2, 3,  4,  5,  6,  7}},
                {{8,  7,  6,  5,  4,  3, 2, 1, 1, 2, 3, 4,  5,  6,  7,  8}},
                {{9,  8,  7,  6,  5,  4, 3, 2, 2, 3, 4, 5,  6,  7,  8,  9}},
                {{10, 9,  8,  7,  6,  5, 4, 3, 3, 4, 5, 6,  7,  8,  9,  10}},
                {{11, 10, 9,  8,  7,  6, 5, 4, 4, 5, 6, 7,  8,  9,  10, 11}},
                {{12, 11, 10, 9,  8,  7, 6, 5, 5, 6, 7, 8,  9,  10, 11, 12}},
                {{13, 12, 11, 10, 9,  8, 7, 6, 6, 7, 8, 9,  10, 11, 12, 13}},
                {{14, 13, 12, 11, 10, 9, 8, 7, 7, 8, 9, 10, 11, 12, 13, 14}},
        }};
    }
};
