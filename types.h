#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

/**
 * Ordered Pair - Int
 * Defines the x and y coordinates of a cell, for the snake body and food pieces.
 * 
 * Top left corner is (0, 0), incrementing down and to the right.
 * 
 * x and y are signed so we can check if the snake is out of bounds
*/
typedef struct {
  int x, y;
} OrderedPairI;

/**
 * Directions that the snake can move in
*/
enum Direction {
  NORTH, SOUTH, EAST, WEST
};

#endif