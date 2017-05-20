#include "Maze.h"
#include <iostream>

using namespace std;

class LeftWallFollower : public PathFinder {
public:
    LeftWallFollower(bool shouldPause = false) : pause(shouldPause) {
        shouldGoForward = false;
        visitedStart = false;
    }

    MouseMovement nextMovement(unsigned x, unsigned y, Maze &maze) {
        const bool frontWall = maze.wallInFront(x,y);
        const bool leftWall  = maze.wallOnLeft(x,y);

        // Pause at each cell if the user requests it.
        // It allows for better viewing on command line.
        if(pause) {
            cout << "Hit enter to continue..." << endl;
            cin.ignore(10000, '\n');
            cin.clear();
        }

        cout << maze.draw(5) << endl << endl;

        // If we somehow miraculously hit the center
        // of the maze, just terminate and celebrate!
        if(isAtCenter(x, y)) {
            cout << "Found center! Good enough for the demo, won't try to get back." << endl;
            return Finish;
        }

        // If we hit the start of the maze a second time, then
        // we couldn't find the center and never will...
        if(x == 0 && y == 0) {
            if(visitedStart) {
                cout << "Unable to find center, giving up." << endl;
                return Finish;
            } else {
                visitedStart = true;
            }
        }

        // If we have just turned left, we should take that path!
        if(!frontWall && shouldGoForward) {
            shouldGoForward = false;
            return MoveForward;
        }

        // As long as nothing is in front and we have
        // a wall to our left, keep going forward!
        if(!frontWall && leftWall) {
            shouldGoForward = false;
            return MoveForward;
        }

        // If our forward and left paths are blocked
        // we should try going to the right!
        if(frontWall && leftWall) {
            shouldGoForward = false;
            return TurnClockwise;
        }

        // Lastly, if there is no left wall we should take that path!
        if(!leftWall) {
            shouldGoForward = true;
            return TurnCounterClockwise;
        }

        // If we get stuck somehow, just terminate.
        cout << "Got stuck..." << endl;
        return Finish;
    }

protected:
    // Helps us determine that we should go forward if we have just turned left.
    bool shouldGoForward;

    // Helps us determine if we've made a loop around the maze without finding the center.
    bool visitedStart;

    // Indicates we should pause before moving to next cell.
    // Useful for command line usage.
    const bool pause;

    bool isAtCenter(unsigned x, unsigned y) const {
        unsigned midpoint = MazeDefinitions::MAZE_LEN / 2;

        if(MazeDefinitions::MAZE_LEN % 2 != 0) {
            return x == midpoint && y == midpoint;
        }

        return  (x == midpoint     && y == midpoint    ) ||
                (x == midpoint - 1 && y == midpoint    ) ||
                (x == midpoint     && y == midpoint - 1) ||
                (x == midpoint - 1 && y == midpoint - 1);
    }
};



int main(int argc, char * argv[]) {
    MazeDefinitions::MazeEncodingName mazeName = MazeDefinitions::MAZE_CAMM_2012;
    bool pause = false;

    // Since Windows does not support getopt directly, we will
    // have to parse the command line arguments ourselves.

    // Skip the program name, start with argument index 1
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-m") == 0 && i+1 < argc) {
            int mazeOption = atoi(argv[++i]);
            if(mazeOption < MazeDefinitions::MAZE_NAME_MAX && mazeOption > 0) {
                    mazeName = (MazeDefinitions::MazeEncodingName)mazeOption;
            }
        } else if(strcmp(argv[i], "-p") == 0) {
            pause = true;
        } else {
            cout << "Usage: " << argv[0] << " [-m N] [-p]" << endl;
            cout << "\t-m N will load the maze corresponding to N, or 0 if invalid N or missing option" << endl;
            cout << "\t-p will wait for a newline in between cell traversals" << endl;
            return -1;
        }
    }

//    LeftWallFollower leftWallFollower(pause);
//    Maze maze(mazeName, &leftWallFollower);
//    cout << maze.draw(5) << endl << endl;
//
//    maze.start();

    PathFinderImpl currMouse(pause);
    Maze maze(mazeName, &currMouse);
    cout << maze.draw(5) << endl << endl;

    maze.start();

}
