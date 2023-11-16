#include <SDL2/SDL.h>
#include "types.h"
#include "constants.h"
#include "logic.h"

/* internal function that handles drawing the window border and X button */
void _render_window_border(SDL_Renderer *r)
{
  SDL_Rect border_left = {
    .x = 0,
    .y = 0,
    .h = WINDOW_HEIGHT_INITIAL,
    .w = WINDOW_BORDER_THICKNESS
  };
  SDL_Rect border_right = {
    .x = WINDOW_WIDTH_INITIAL - WINDOW_BORDER_THICKNESS,
    .y = 0,
    .h = WINDOW_HEIGHT_INITIAL,
    .w = WINDOW_BORDER_THICKNESS
  };
  SDL_Rect border_top = {
    .x = 0,
    .y = 0,
    .h = WINDOW_BORDER_THICKNESS,
    .w = WINDOW_WIDTH_INITIAL
  };
  SDL_Rect border_bottom = {
    .x = 0,
    .y = WINDOW_HEIGHT_INITIAL - WINDOW_BORDER_THICKNESS,
    .h = WINDOW_BORDER_THICKNESS,
    .w = WINDOW_WIDTH_INITIAL
  };
  SDL_SetRenderDrawColor(r, WINDOW_BORDER_COLOR, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(r, &border_left);
  SDL_RenderFillRect(r, &border_right);
  SDL_RenderFillRect(r, &border_top);
  SDL_RenderFillRect(r, &border_bottom);
}

void _render_window_menu(SDL_Renderer *r)
{
  int mouse_x, mouse_y;
  SDL_Rect menu_bottom_border = {
    .x = 0,
    .y = WINDOW_BORDER_THICKNESS + MENU_HEIGHT,
    .w = WINDOW_WIDTH_INITIAL,
    .h = WINDOW_BORDER_THICKNESS
  };
  SDL_Rect menu_exit_button = {
    .x = MENU_EXIT_BUTTON_X,
    .y = MENU_EXIT_BUTTON_Y,
    .w = MENU_EXIT_BUTTON_W,
    .h = MENU_EXIT_BUTTON_H
  };
  SDL_Rect menu_exit_button_border_left = {
    .x = MENU_EXIT_BUTTON_X - WINDOW_BORDER_THICKNESS,
    .y = WINDOW_BORDER_THICKNESS,
    .w = WINDOW_BORDER_THICKNESS,
    .h = MENU_EXIT_BUTTON_H
  };
  SDL_SetRenderDrawColor(r, WINDOW_BORDER_COLOR, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(r, &menu_bottom_border);
  SDL_RenderFillRect(r, &menu_exit_button_border_left);
  SDL_GetMouseState(&mouse_x, &mouse_y);
  if (is_mouse_over_exit_button(mouse_x, mouse_y))
    SDL_SetRenderDrawColor(r, MENU_EXIT_BUTTON_COLOR_HOVER, SDL_ALPHA_OPAQUE);
  else
    SDL_SetRenderDrawColor(r, MENU_EXIT_BUTTON_COLOR, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(r, &menu_exit_button);
}

/* Draws a grid according to GRID_COUNT_X, GRID_COUNT_Y, GRID_CELL_WIDTH, and GRID_CELL_HEIGHT */
/* important: grid cell width and height should be calculated as a float, and then case to an int
 *   just before drawing line, to ensure no loss of width or height */
void _render_grid(SDL_Renderer *r)
{
  unsigned int i;
  float x1, y1, x2, y2;

  SDL_SetRenderDrawColor(r, GRID_COLOR, SDL_ALPHA_OPAQUE);

  for (i = 1; i < GRID_COUNT_X; i++) {
    x1 = GRID_X + (i * GRID_CELL_WIDTH);
    y1 = GRID_Y;
    x2 = GRID_X + (i * GRID_CELL_WIDTH);
    y2 = GRID_Y + GRID_HEIGHT;
    SDL_RenderDrawLine(r, (int)x1, (int)y1, (int)x2, (int)y2);
  }
  for (i = 1; i < GRID_COUNT_Y; i++) {
    x1 = GRID_X;
    y1 = GRID_Y + (i * GRID_CELL_HEIGHT);
    x2 = GRID_X + GRID_WIDTH;
    y2 = GRID_Y + (i * GRID_CELL_HEIGHT);
    SDL_RenderDrawLine(r, (int)x1, (int)y1, (int)x2, (int)y2);    
  }
}

void _render_snake(SDL_Renderer *r, Snake *s)
{
  SDL_SetRenderDrawColor(r, SNAKE_COLOR, SDL_ALPHA_OPAQUE);
  int length = s->length;
  for (int i = 0; i < length; i++) {
    SnakeSegment seg = *(s->segments + i);
    SDL_Rect rect = {
      .x = (int) ((seg.x * GRID_CELL_WIDTH) + GRID_X),
      .y = (int) ((seg.y * GRID_CELL_HEIGHT) + GRID_Y),
      .w = (int) (GRID_CELL_WIDTH + 0.5f), /* round up to the nearest pixel */
      .h = (int) (GRID_CELL_HEIGHT + 0.5f)
    };
    SDL_RenderFillRect(r, &rect);
  }
}

void _render_food(SDL_Renderer *r, Food f)
{
  SDL_Rect food_rect = {
    .x = (int) ((f.x * GRID_CELL_WIDTH) + GRID_X),
    .y = (int) ((f.y * GRID_CELL_HEIGHT) + GRID_Y),
    .w = (int) GRID_CELL_WIDTH + 0.5f,
    .h = (int) GRID_CELL_HEIGHT + 0.5f
  };
  SDL_SetRenderDrawColor(r, FOOD_COLOR, SDL_ALPHA_OPAQUE);
  SDL_RenderFillRect(r, &food_rect);
}

void _render_pause_overlay(SDL_Renderer *r, bool is_paused)
{
  if (is_paused) {
    SDL_Rect overlay_rect = {
      .x = GRID_X,
      .y = GRID_Y,
      .w = GRID_WIDTH,
      .h = GRID_HEIGHT
    };
    SDL_SetRenderDrawColor(r, OVERLAY_PAUSE_COLOR, 0x80);
    SDL_RenderFillRect(r, &overlay_rect);
  }
}

void _render_snake_dead(SDL_Renderer *r, bool snake_is_alive)
{
  if (snake_is_alive == false) {
    SDL_Rect overlay_rect = {
      .x = GRID_X,
      .y = GRID_Y,
      .w = GRID_WIDTH,
      .h = GRID_HEIGHT
    };
    SDL_SetRenderDrawColor(r, OVERLAY_DEAD_COLOR, 0x80);
    SDL_RenderFillRect(r, &overlay_rect);
  }
}

/* render function is only responsible for drawing game objects */
void render(GameState *state)
{
  SDL_Renderer *r = state->renderer;

  /* clear screen to black */
  SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
  SDL_RenderClear(r);

  /* draw menu first, so that out-of-bounds drawing is visible */
  _render_window_border(state->renderer);
  _render_window_menu(state->renderer);
  _render_snake(state->renderer, state->snake);
  _render_food(state->renderer, state->food);
  _render_grid(state->renderer);
  _render_pause_overlay(state->renderer, state->is_paused);
  _render_snake_dead(state->renderer, state->snake->is_alive);
  
  /* swap the buffers */
  SDL_RenderPresent(r);
}
