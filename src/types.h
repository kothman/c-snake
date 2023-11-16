#include <stdbool.h>
#include <SDL2/SDL.h>

#ifndef SNAKE_TYPES_H
#define SNAKE_TYPES_H

typedef enum Directions {NORTH, SOUTH, EAST, WEST} Direction;

typedef struct {
  unsigned int x;
  unsigned int y;
} SnakeSegment, Food, Point;

typedef struct {
  SnakeSegment *segments;
  SnakeSegment segment_tail_previous;
  unsigned int length;
  unsigned int segment_buffer_size;
  Uint64 move_delay_ms;
  Uint64 last_move_ms;
  Direction direction;
  Direction direction_queued;
  bool is_alive;
  bool should_reset; /* Game has been unpaused after snake death */
} Snake;

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  bool is_running;
  bool is_paused;
  unsigned int score;
  Snake *snake;
  Food food;
} GameState;

#endif /* SNAKE_TYPES_H */