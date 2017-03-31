#ifndef Maze_h
#define Maze_h

#include <string>

#include "BitVector256.h"
#include "MazeDefinitions.h"
#include "Dir.h"
#include "PathFinder.h"

class Maze {
protected:
    BitVector256 wallNS;
    BitVector256 wallEW;
    Dir heading;
    PathFinder *pathFinder;
    unsigned mouseX;
    unsigned mouseY;

    bool isOpen(unsigned x, unsigned y, Dir d) const;

    void setOpen(unsigned x, unsigned y, Dir d);

    void moveForward();

    void moveBackward();

    inline void turnClockwise() {
        heading = clockwise(heading);
    }

    inline void turnCounterClockwise() {
        heading = counterClockwise(heading);
    }

    inline void turnAround() {
        heading = opposite(heading);
    }

public:
    Maze(MazeDefinitions::MazeEncodingName name, PathFinder *pathFinder);

    Dir getCurrentDirection() const {
        return heading;
    }

    void setCurrentDirection(Dir d) {
        switch (d) {
            case NORTH:
                if (heading == NORTH) {
                    break;
                }
                if (heading == SOUTH) {
                    turnAround();
                    break;
                }
                if (heading == EAST) {
                    turnCounterClockwise();
                    break;
                }
                if (heading == WEST) {
                    turnClockwise();
                    break;
                }
                break;
            case SOUTH:
                if (heading == NORTH) {
                    turnAround();
                    break;
                }
                if (heading == SOUTH) {
                    break;
                }
                if (heading == EAST) {
                    turnClockwise();
                    break;
                }
                if (heading == WEST) {
                    turnCounterClockwise();
                    break;
                }
                break;
            case EAST:
                if (heading == NORTH) {
                    turnClockwise();
                    break;
                }
                if (heading == SOUTH) {
                    turnCounterClockwise();
                    break;
                }
                if (heading == EAST) {
                    break;
                }
                if (heading == WEST) {
                    turnAround();
                    break;
                }
                break;
            case WEST:
                if (heading == NORTH) {
                    turnCounterClockwise();
                    break;
                }
                if (heading == SOUTH) {
                    turnClockwise();
                    break;
                }
                if (heading == EAST) {
                    turnAround();
                    break;
                }
                if (heading == WEST) {
                    break;
                }
                break;
        }
    }

    inline bool wallInFront() const {
        return !isOpen(mouseX, mouseY, heading);
    }

    inline bool wallOnLeft() const {
        return !isOpen(mouseX, mouseY, counterClockwise(heading));
    }

    inline bool wallOnRight() const {
        return !isOpen(mouseX, mouseY, clockwise(heading));
    }

    inline bool wallInBack() const {
        return !isOpen(mouseX, mouseY, opposite(heading));
    }

    /**
     * Start running the mouse through the maze.
     * Terminates when the PathFinder's nextMovement method returns MouseMovement::Finish.
     */
    void start();

    /**
     * This function draws the maze using ASCII characters.
     *
     * Queries the underlying PathFinder for additional maze info
     * and incorporates it in the maze rendering.
     * @param infoLen: specifies the max characters of info to be drawn. If no info is supplied, blank spaces will be inserted.
     * @return string of rendered maze
     */
    std::string draw(const size_t infoLen = 4) const;
};

#endif
