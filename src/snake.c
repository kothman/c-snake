#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "constants.h"
#include "types.h"

#define SNAKE_INITIAL_LENGTH 4
#define SNAKE_INITIAL_BUFFER_MULTIPLE 64
#define SNAKE_INITIAL_MOVE_DELAY_MS 500



Snake * snake_initialize(void)
{
  Snake *s = NULL;
  s = malloc(sizeof(Snake));
  if (s == NULL) {
    fprintf(stderr, "[error]: Failed to allocate memory for Snake\n");
    return NULL;
  }
  s->segments = malloc(SNAKE_INITIAL_BUFFER_MULTIPLE * sizeof(SnakeSegment));
  if (s->segments == NULL) {
    fprintf(stderr, "[error]: Failed to allocate memory for SnakeSegments\n");
    return NULL;
  }

  s->length = SNAKE_INITIAL_LENGTH;
  s->segment_buffer_size = SNAKE_INITIAL_BUFFER_MULTIPLE * sizeof(SnakeSegment);
  s->direction = EAST;
  s->direction_queued = EAST;
  s->move_delay_ms = SNAKE_INITIAL_MOVE_DELAY_MS;
  s->last_move_ms = SDL_GetTicks64() + SNAKE_INITIAL_MOVE_DELAY_MS;
  s->is_alive = true;
  s->should_reset = false;
  
  /* set the initial snake segments */
  for (int i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
    (s->segments + i)->x = SNAKE_INITIAL_LENGTH - i - 1;
    (s->segments + i)->y = GRID_COUNT_Y / 2;
  }
  return s;
}

void snake_deinitialize(Snake *s)
{
  if (s == NULL) {
    fprintf(stderr, "[error]: Tried to deallocate NULL Snake pointer\n");
    return;
  }
  if (s->segments == NULL) {
    fprintf(stderr, "[error]: Tried to deallocate NULL Snake->segments pointer\n");
    return;
  }
  free(s->segments);
  free(s);
}
