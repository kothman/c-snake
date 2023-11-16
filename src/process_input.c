#include <SDL2/SDL.h>
#include "types.h"
#include "logic.h"

void _process_keydown_event(GameState *state, SDL_Keycode sym)
{
  switch (sym) {
  case SDLK_q:
  case SDLK_ESCAPE:
    state->is_running = false;
    break;
  case SDLK_p:
  case SDLK_SPACE:
    if (state->snake->is_alive == false)
      state->snake->should_reset = true;
    else
      state->is_paused = !state->is_paused;
    break;
  case SDLK_UP:
    if (state->snake->direction != SOUTH)
      state->snake->direction_queued = NORTH;
    break;
  case SDLK_DOWN:
    if (state->snake->direction != NORTH)
      state->snake->direction_queued = SOUTH;
    break;
  case SDLK_LEFT:
    if (state->snake->direction != EAST)
      state->snake->direction_queued = WEST;
    break;
  case SDLK_RIGHT:
    if (state->snake->direction != WEST)
      state->snake->direction_queued = EAST;
    break;
  }
}

/* process_input function is only responsible for handling user input */
void process_input(GameState *state)
{
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
    case SDL_QUIT:
      state->is_running = false;
      break;
    /* handle MOUSEBUTTONUP event */
    case SDL_MOUSEBUTTONUP:
      if (e.button.button == SDL_BUTTON_LEFT && is_mouse_over_exit_button(e.button.x, e.button.y))
	state->is_running = false;
      break;
    /* handle KEYDOWN events */
    case SDL_KEYDOWN:
      _process_keydown_event(state, e.key.keysym.sym);
    default:
      break;
    } /* END switch(e.type) */
  } /* END while (SDL_PollEvent(&e)) */
}
