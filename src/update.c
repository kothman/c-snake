#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "types.h"
#include "logic.h"

Snake * snake_initialize(void);
void snake_deinitialize(Snake *s);

void _update_snake_position(Snake *snake)
{
  unsigned int s_length = snake->length;

  /* is it time to move the snake? */
  if (SDL_GetTicks64() >= snake->last_move_ms + snake->move_delay_ms) {
    /* use a queued direction to prevent doubling back on self */
    snake->direction = snake->direction_queued;
    /* save the tail segment in case the snake eats some food */
    SnakeSegment *segment_tail_previous = (snake->segments + snake->length - 1);
    snake->segment_tail_previous.x = segment_tail_previous->x;
    snake->segment_tail_previous.y = segment_tail_previous->y;

    for (unsigned int i = s_length; i > 0; i--) {
      SnakeSegment *seg = snake->segments + i -1;
      if (i - 1 == 0) { /* head element gets moved in direction */
        switch (snake->direction) {
        case NORTH:
          seg->y--;
          break;
        case EAST:
          seg->x++;
          break;
        case SOUTH:
          seg->y++;
          break;
        case WEST:
          seg->x--;
          break;
        default: /* Something bad happened */
          fprintf(stderr, "[error]: unknown direction %i\n", snake->direction);
          break;
        }
      } else { /* Not the head element, so move it to the position of element -1 index in array */
        seg->x = (seg - 1)->x;
        seg->y = (seg - 1)->y;
      }
    } /* end for loop */

    snake->last_move_ms = SDL_GetTicks64();
  } /* end if update snake position */

}

void _update_randomize_food_location(Food *food, Snake *snake) {
  Food new_food = {0, 0};
  do {
    new_food.x = rand() % GRID_COUNT_X;
    new_food.y = rand() % GRID_COUNT_Y;
  } while (is_point_in_array(&new_food, snake->segments, snake->length));
  food->x = new_food.x;
  food->y = new_food.y;
}

void _update_snake_eat_food(Snake *snake, Food *food) {
  /* does snake head position == food position? */
  if (snake->segments->x == food->x && snake->segments->y == food->y) {
    snake->length++;
    /* realloc snake->segments if needed */
    if (snake->length * sizeof(SnakeSegment) >= snake->segment_buffer_size) {
      snake->segment_buffer_size *= 2;
      snake->segments = realloc(snake->segments, snake->segment_buffer_size);
      if (snake->segments == NULL) {
	      fprintf(stderr, "[error]: failed to reallocate space for snake segments");
      }
    } /* endif realloc snake->segments */
    SnakeSegment *tail = (snake->segments + snake->length - 1);
    tail->x = snake->segment_tail_previous.x;
    tail->y = snake->segment_tail_previous.y;

    _update_randomize_food_location(food, snake);

    if (snake->move_delay_ms - SNAKE_MOVE_DELAY_DECREMENT_MS >= SNAKE_MOVE_DELAY_MIN_MS)
      snake->move_delay_ms -= SNAKE_MOVE_DELAY_DECREMENT_MS;
  } /* endif snake head position == food position */
}

bool _incoming_collision(Snake *s)
{
  Point head = *(s->segments);
  Point upcoming_position = s->segments[0];
  switch (s->direction_queued) {
    case NORTH:
      if (head.y == 0) return true;
      upcoming_position.y--;
      break;
    case SOUTH:
      if (head.y + 1 == GRID_COUNT_Y) return true;
      upcoming_position.y++;
      break;
    case EAST:
      if (head.x + 1 == GRID_COUNT_X) return true;
      upcoming_position.x++;
      break;
    case WEST:
      if (head.x == 0) return true;
      upcoming_position.x--;
      break;
    default:
      fprintf(stderr, "[error]: unknown enum Direction [%i] in snake->direction_queued\n", s->direction_queued);
      break;
  }
  if (is_point_in_array(&upcoming_position, s->segments, s->length))
    return true;
  
  return false;
}

Snake * _update_reset_snake(Snake *s)
{
  snake_deinitialize(s);
  return snake_initialize();
}

/* update function is only responsible for handling game logic */
void update(GameState *state)
{
  Snake *snake = state->snake;
  Food *food = &(state->food);
  
  /* wait for process_input to restart game */
  if (snake->should_reset) {
    snake = _update_reset_snake(snake);
    state->snake = snake;
  }
    
  if (snake->is_alive == false)
    return;

  if (state->is_paused) {
    /* updated snake->last_move_ms to provide delay before unpausing */
    snake->last_move_ms = SDL_GetTicks64();
    return;
  }

  if (_incoming_collision(snake)) {
    snake->is_alive = false;
    return;
  }
  _update_snake_position(snake);
  _update_snake_eat_food(snake, food);
  
  /* update snake position */
  
}
